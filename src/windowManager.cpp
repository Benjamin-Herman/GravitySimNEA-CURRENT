#include "../headers/windowManager.h"
#include <iostream>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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

GLFWwindow* windowManager::createWindow(unsigned int width, unsigned int height, const char* screenName, GLFWwindow* share, bool isGUI){
    // First window creation (main window)
    if (!share) {
        if (!glfwInit()) {
            throw std::runtime_error("GLFW initialization failed");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    // Create window (automatically shares context if 'share' is not null)
    GLFWwindow* window = glfwCreateWindow(width, height, screenName, nullptr, share);
    if (!window) {
        if (!share) glfwTerminate();
        throw std::runtime_error("Window creation failed");
    }

    // Only initialize context for main window
    if (!share) {
        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            glfwTerminate();
            throw std::runtime_error("GLAD initialization failed");
        }
    }

    return window;
}

