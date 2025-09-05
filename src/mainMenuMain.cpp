#include "../headers/mainMenuMain.h"
#include "../headers/windowManager.h"
#include "../headers/GUI.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

int mainMenuClass::mainMenuCall() {
    window_Manager.activateGLFW(); //activate glfw
    GLFWwindow* window = window_Manager.createWindow(800, 600, "MAIN MENU"); 
    if (!window) {
        return -1;
    }

    glEnable(GL_BLEND); //allows to blend over 3d viewport
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GUI gui;
    if (!gui.loadFont("fonts/font2.ttf", 48.0f)) { //loads font
        std::cout << "Failed to load font\n";
        return -1;
    }
    int Cwidth, Cheight; //C stands for current
    glm::vec2 middle;
    glm::vec2 topMiddle;
    
    GUI::anchors _anchors;
    
    while (!glfwWindowShouldClose(window)) {
        std::vector<glm::vec2> anchors;
        gui.updateSize(window); //updates size so that everything works with resizing
        _anchors = gui._anchors;
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //clears the backround
        glClear(GL_COLOR_BUFFER_BIT);
        glfwGetWindowSize(window, &Cwidth, &Cheight);
        
        glm::vec2 titleOffset = { 120.f, -100.f };
        glm::vec2 btnOffset = { 140.f, -150.f };
        glm::vec2 txt2Offset = { 230.f, 100.f };
        //TITLE TEXT
        gui.renderText("MAIN MENU", _anchors.topMiddle - titleOffset, 1.0f, glm::vec3{1.f}); //text to render
        //EXIT TEXT
        //gui.renderText("Press ESC to quit", _anchors.bottomMiddle - txt2Offset, 1.0f, glm::vec3{ 1.f });
        //BUTTON
        gui.renderButton("I AM BUTTON", _anchors.topMiddle - btnOffset, 1.0f, glm::vec3{ 1.f }, glm::vec2{ 350.f, 75.f }, glm::vec3{ 1.f, 0.f, 0.f }, glm::vec2{ 10.f, 50.f }, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}