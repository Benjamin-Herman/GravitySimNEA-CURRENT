#include "../headers/windowManager.h"
#include "../headers/screenData.h"
#include <glad/glad.h>
#include <iostream>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); //just base class to return of null
}

void windowManager::activateGLFW() {
    // GLFW init
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
    }
    //tell openGL what version cuz it somehow doesnt know
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow* windowManager::createWindow() {
    
    // create window
    GLFWwindow* window = glfwCreateWindow(width, height, screenName, NULL, NULL);
    if (!window) { //failed window
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window); //tell openGL this is the main window to render
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLAD init
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
    }

    return window;
}

