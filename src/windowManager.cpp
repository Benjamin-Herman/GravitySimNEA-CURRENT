#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "../headers/windowManager.h"
#include "../headers/screenData.h"
//#include <glad/glad.h>
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

GLFWwindow* windowManager::createWindow(unsigned int iwidth, unsigned int iheight, const char* iscreenName) {
    if (iwidth == 0) iwidth = width;
    if (iheight == 0) iheight = height;
    if (!iscreenName) iscreenName = screenName;
    // create window
    GLFWwindow* window = glfwCreateWindow(iwidth, iheight, iscreenName, NULL, NULL);
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

    //imgui implementation
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    return window;
}

