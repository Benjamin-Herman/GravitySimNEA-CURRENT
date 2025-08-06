#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "../headers/cube.h"
#include "../headers/sphere.h"
#include "../headers/shader.h"
#include "../headers/camera.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void processArrowKeys(GLFWwindow* window, float deltaTime);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
float getDeltaTime();
void CreateStarfield(unsigned int& VAO, unsigned int& VBO, int starCount = 1000);

// Global variables
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
unsigned int starVAO, starVBO;

int main() {
    // Initialize random seed
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // GLFW initialization
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Space Simulator", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLAD initialization
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // Input callbacks
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Configure global OpenGL state
    glEnable(GL_DEPTH_TEST);

    // Build and compile shaders
    Shader objectShader("shaders/shader.vert", "shaders/shader.frag");
    Shader starShader("shaders/starShader.vert", "shaders/starShader.frag");

    // Create starfield
    CreateStarfield(starVAO, starVBO, 5000);

    // Create objects
    Sphere sphere(0.8f);
    // Cube cube; // Uncomment if you want to use the cube

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        float deltaTime = getDeltaTime();

        // Input processing
        processInput(window);
        processArrowKeys(window, deltaTime);
        camera.ProcessKeyboard(window, deltaTime);

        // Clear screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Pure black for space
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Create projection matrix
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            (float)SCR_WIDTH / (float)SCR_HEIGHT,
            0.1f,
            100.0f
        );

        // Draw starfield (no depth testing)
        glDisable(GL_DEPTH_TEST);
        starShader.Use();
        starShader.SetMat4("view", glm::mat4(glm::mat3(camera.GetViewMatrix())));
        starShader.SetMat4("projection", projection);
        glBindVertexArray(starVAO);
        glDrawArrays(GL_POINTS, 0, 5000);
        glEnable(GL_DEPTH_TEST);

        // Draw objects
        objectShader.Use();
        objectShader.SetMat4("projection", projection);
        objectShader.SetMat4("view", camera.GetViewMatrix());

        // Set lighting uniforms
        glm::vec3 lightPos(2.0f, 2.0f, 2.0f);
        objectShader.SetVec3("lightPos", lightPos);
        objectShader.SetVec3("viewPos", camera.Position);

        // Draw sphere
        sphere.Update(deltaTime);
        objectShader.SetMat4("model", sphere.GetModelMatrix());
        sphere.Render();

        /*
        // Uncomment to draw cube
        cube.Update(deltaTime);
        objectShader.SetMat4("model", cube.GetModelMatrix());
        cube.Render();
        */

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &starVAO);
    glDeleteBuffers(1, &starVBO);
    glfwTerminate();
    return 0;
}

// Function implementations
void CreateStarfield(unsigned int& VAO, unsigned int& VBO, int starCount) {
    std::vector<float> vertices;

    for (int i = 0; i < starCount; ++i) {
        float theta = 2.0f * glm::pi<float>() * (rand() / (float)RAND_MAX);
        float phi = acos(1.0f - 2.0f * (rand() / (float)RAND_MAX));
        float r = 50.0f;

        float x = r * sin(phi) * cos(theta);
        float y = r * sin(phi) * sin(theta);
        float z = r * cos(phi);

        float brightness = 0.5f + (rand() / (float)RAND_MAX) * 0.5f;

        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
        vertices.push_back(brightness);
        vertices.push_back(brightness);
        vertices.push_back(brightness);
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

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

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}