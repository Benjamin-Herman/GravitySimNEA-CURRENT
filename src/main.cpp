#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "../headers/cube.h"
#include "../headers/sphere.h"
#include "../headers/shader.h"
#include "../headers/camera.h"







const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Global camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Mouse callback
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


float getDeltaTime() {
    static float lastTime = glfwGetTime();
    float currentTime = glfwGetTime();
    float dt = currentTime - lastTime;
    lastTime = currentTime;
    return dt;
}
void processArrowKeys(GLFWwindow* window, float deltaTime) {
    float sensitivity = 50.0f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.ProcessArrowKeys(-sensitivity, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.ProcessArrowKeys(sensitivity, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.ProcessArrowKeys(0.0f, sensitivity);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.ProcessArrowKeys(0.0f, -sensitivity);
}


int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Gravity Simulator", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Configure global OpenGL state
    glEnable(GL_DEPTH_TEST);

    // Build and compile shaders
    Shader Shader("shaders/shader.vert", "shaders/shader.frag");

    // Create cube
    Cube cube;

    // Create sphere
    Sphere sphere(0.8f); //size 80% of cube

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        float deltaTime = getDeltaTime();

        // Input
        processInput(window);
        camera.ProcessKeyboard(window, deltaTime);  // Pass the window pointer
        processArrowKeys(window, deltaTime); // Add this line


        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate shader
        Shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        Shader.SetMat4("view", view);

        // Create transformations
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        // Pass transformation matrices to the shader
        Shader.SetMat4("projection", projection);
        Shader.SetMat4("model", cube.GetModelMatrix());

        // Update and render cube
        cube.Update(deltaTime);
        cube.Render();

        Shader.SetMat4("model", sphere.GetModelMatrix());
        sphere.Update(deltaTime);
        sphere.Render();

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}