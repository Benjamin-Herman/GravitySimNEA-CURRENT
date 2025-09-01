#include "../headers/windowManager.h"
#include <iostream>
#include <string>
#include <GLFW/glfw3.h>


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

windowManager window_Manager;

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

void windowManager::showFPS() {
    using clock = std::chrono::high_resolution_clock;
    static int frameCount = 0;
    static auto lastTime = clock::now();

    frameCount++;
    auto now = clock::now();
    std::chrono::duration<double> elapsed = now - lastTime;

    if (elapsed.count() >= 0.2) {
        double fps = frameCount / elapsed.count();
        
        std::string fpsStr = std::string(name) + " | FPS: " + std::to_string(fps);
        const char* cstr = fpsStr.c_str();

        glfwSetWindowTitle(window, fpsStr.c_str());

        frameCount = 0;
        lastTime = now;
    }

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
    window = glfwCreateWindow(width, height, screenName, nullptr, share);
    if (!window) {
        if (!share) glfwTerminate();
        throw std::runtime_error("Window creation failed");
    }

    // Only initialize context for main window
    if (!share) {
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            glfwTerminate();
            throw std::runtime_error("GLAD initialization failed");
        }
    }
    name = glfwGetWindowTitle(window);
    //glfwSwapInterval(0);
    return window;
}

