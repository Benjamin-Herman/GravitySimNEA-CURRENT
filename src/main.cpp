#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "../headers/includes.h"
#include "../headers/cube.h"
#include "../headers/sphere.h"
#include "../headers/shader.h"
#include "../headers/camera.h"
#include "../headers/graphicsManager.h"
#include "../headers/inputManager.h"
#include "../headers/screenData.h"
#include "../headers/time.h"


// func declarations to access anywhere in file
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// couple globals
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
unsigned int starVAO, starVBO;

int main() {
    //random seed for the stars. no clue why its still in main.cpp
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // GLFW init
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return -1;
    }
    //tell openGL what version cuz it somehow doesnt know
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window
    GLFWwindow* window = glfwCreateWindow(width, height, screenName, NULL, NULL);
    if (!window) { //failed window
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); //tell openGL this is the main window to render
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLAD init
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // input callback thingys
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // enable depth testing
    glEnable(GL_DEPTH_TEST);

    // compile shaders
    std::vector<Shader> shaders;
    Shader objectShader("shaders/shader.vert", "shaders/shader.frag");
    Shader starShader("shaders/starShader.vert", "shaders/starShader.frag");
    shaders.push_back(objectShader);
    shaders.push_back(starShader);


    // make starsssss
    CreateStarfield(starVAO, starVBO, 5000);

    // make objects
    Sphere sphere(0.8f);
    Cube cube; // uncomment for cube stuff

    // render loop. TODO move to graphics manager
    while (!glfwWindowShouldClose(window)) {
        float deltaTime = Time::DeltaTime(); //get delta time

        //process input commands. basically say i call youuu
        processInput(window);
        processArrowKeys(window, deltaTime, camera);
        camera.ProcessKeyboard(window, deltaTime);

        // clear screen. comment out for fun looking screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // ultra black for space
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // make projection so the view is like you eyes
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f), //internet maths
            (float)width / (float)height,
            0.1f,
            100.0f
        );

        // draw stars with no depth test
        glDisable(GL_DEPTH_TEST);
        //use the shader code
        starShader.Use();
        starShader.SetMat4("view", glm::mat4(glm::mat3(camera.GetViewMatrix())));
        starShader.SetMat4("projection", projection);
        glBindVertexArray(starVAO);
        glDrawArrays(GL_POINTS, 0, 5000); //say 5000 stars. Change for fun
        glEnable(GL_DEPTH_TEST); //reenable becuase depth test is kind of important

        // draw stuff
        objectShader.Use(); //use shaders and use matricies
        objectShader.SetMat4("projection", projection);
        objectShader.SetMat4("view", camera.GetViewMatrix());

        // set a light up so stuff can be seen, otherwise pretty much all black
        glm::vec3 lightPos(2.0f, 2.0f, 2.0f);
        objectShader.SetVec3("lightPos", lightPos);
        objectShader.SetVec3("viewPos", camera.Position);

        // draw sphere
        sphere.Update(deltaTime);
        objectShader.SetMat4("model", sphere.GetModelMatrix());
        sphere.Render();

        ///*
        // uncomment to draw cube
        cube.Update(deltaTime);
        objectShader.SetMat4("model", cube.GetModelMatrix());
        cube.Render();
        //*/

        // swap front and back buffer so new screen and reset poll events. 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // memeory clean becuase cpp sucks
    glDeleteVertexArrays(1, &starVAO);
    glDeleteBuffers(1, &starVBO);
    glfwTerminate();
    return 0;
}



void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    mouseInput(window, xpos, ypos, camera); //call the function elsewhere. needed this for openGL cheats
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); //basiclaly the blank slate
}