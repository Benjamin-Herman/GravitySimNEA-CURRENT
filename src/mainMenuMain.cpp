#include "../headers/mainMenuMain.h"
#include "../headers/windowManager.h"
#include "../headers/gravityMain.h"
#include "../headers/GUI.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include <thread>
#include <chrono>



void clear(GLFWwindow* window) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void renderMainMenuFront(GUI& gui, const GUI::anchors& _anchors, GLFWwindow* window, mainMenuClass::states* _currentState) {

    //create buttons
    button beginBtn(&gui);
    beginBtn.setOnClick([window]() {
        clear(window);
        gravityMain gCntr;
        
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        glfwTerminate();
        gCntr.gravitySimMain();
        glfwTerminate();
        exit(0);
        });

    button optionsBtn(&gui);
    optionsBtn.setOnClick([_currentState]() {
        *_currentState = mainMenuClass::states::optionsMenu;
        });

    button quitBtn(&gui);
    quitBtn.setOnClick([window]() {  //must capture window. some local scope thing
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        });

    glm::vec2 titleOffset = { 120.f, -100.f };
    glm::vec2 btnOffset = { 140.f, -150.f };
    glm::vec2 txt2Offset = { 230.f, 100.f };

    //TITLE TEXT
    gui.renderText("MAIN MENU", _anchors.topMiddle - titleOffset, 1.0f, glm::vec3{ 1.f }); //text to render

    //EXIT TEXT
    //gui.renderText("Press ESC to quit", _anchors.bottomMiddle - txt2Offset, 1.0f, glm::vec3{ 1.f });

    //BUTTON
    beginBtn.renderButton(
        "BEGIN",                         // text
        _anchors.topMiddle - btnOffset,        // position
        1.0f,                                  // font scale
        glm::vec3{ 1.f },                      // font colour
        glm::vec2{ 350.f, 75.f },              // button size
        glm::vec3{ 1.f, 0.f, 0.f },            // button colour
        glm::vec2{ 10.f, 50.f }               // text offset
    );

    optionsBtn.renderButton(
        "OPTIONS",                         // text
        _anchors.topMiddle - btnOffset - glm::vec2{ 0.f, -150.f },        // position
        1.0f,                                  // font scale
        glm::vec3{ 1.f },                      // font colour
        glm::vec2{ 350.f, 75.f },              // button size
        glm::vec3{ 1.f, 0.f, 0.f },            // button colour
        glm::vec2{ 10.f, 50.f }               // text offset
    );

    quitBtn.renderButton(
        "QUIT",                         // text
        _anchors.topMiddle - btnOffset - glm::vec2{ 0.f, -300.f },        // position
        1.0f,                                  // font scale
        glm::vec3{ 1.f },                      // font colour
        glm::vec2{ 350.f, 75.f },              // button size
        glm::vec3{ 1.f, 0.f, 0.f },            // button colour
        glm::vec2{ 10.f, 50.f }               // text offset
    );
}

void renderOptionsMenu(GUI& gui, const GUI::anchors& _anchors, GLFWwindow* window, mainMenuClass::states* _currentState) {
    //create buttons
    /*button beginBtn(&gui);
    beginBtn.setOnClick([window]() {
        clear(window);
        gravityMain gCntr;

        glfwSetWindowShouldClose(window, GLFW_TRUE);
        glfwTerminate();
        gCntr.gravitySimMain();
        glfwTerminate();
        exit(0);
        });

    button optionsBtn(&gui);
    optionsBtn.setOnClick([_currentState]() {
        *_currentState = mainMenuClass::states::optionsMenu;
        std::cout << "OPTIONS\n";
        });*/

    button backBtn(&gui);
    backBtn.setOnClick([_currentState]() {  //must capture window. some local scope thing
        *_currentState = mainMenuClass::states::main;
        });

    glm::vec2 titleOffset = { 120.f, -100.f };
    glm::vec2 btnOffset = { 140.f, -150.f };
    glm::vec2 txt2Offset = { 230.f, 100.f };

    //TITLE TEXT
    gui.renderText("OPTIONS MENU", _anchors.topMiddle - titleOffset, 1.0f, glm::vec3{ 1.f }); //text to render

    //EXIT TEXT
    //gui.renderText("Press ESC to quit", _anchors.bottomMiddle - txt2Offset, 1.0f, glm::vec3{ 1.f });

    //BUTTON
    /*beginBtn.renderButton(
        "BEGIN",                         // text
        _anchors.topMiddle - btnOffset,        // position
        1.0f,                                  // font scale
        glm::vec3{ 1.f },                      // font colour
        glm::vec2{ 350.f, 75.f },              // button size
        glm::vec3{ 1.f, 0.f, 0.f },            // button colour
        glm::vec2{ 10.f, 50.f }               // text offset
    );

    optionsBtn.renderButton(
        "OPTIONS",                         // text
        _anchors.topMiddle - btnOffset - glm::vec2{ 0.f, -150.f },        // position
        1.0f,                                  // font scale
        glm::vec3{ 1.f },                      // font colour
        glm::vec2{ 350.f, 75.f },              // button size
        glm::vec3{ 1.f, 0.f, 0.f },            // button colour
        glm::vec2{ 10.f, 50.f }               // text offset
    );*/

    backBtn.renderButton(
        "BACK",                         // text
        _anchors.topMiddle - btnOffset - glm::vec2{ 0.f, -300.f },        // position
        1.0f,                                  // font scale
        glm::vec3{ 1.f },                      // font colour
        glm::vec2{ 350.f, 75.f },              // button size
        glm::vec3{ 1.f, 0.f, 0.f },            // button colour
        glm::vec2{ 10.f, 50.f }               // text offset
    );
}

int mainMenuClass::mainMenuCall() {
    currentState = states::main; //allows for all computation of which ui to display here instead of passing variables everywhere

    window_Manager.activateGLFW(); //activate glfw
    GLFWwindow* window = window_Manager.createWindow(800, 600, "MAIN MENU");
    if (!window) {
        return -1;
    }
    glEnable(GL_BLEND); //allows to blend over 3d viewport
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GUI gui(window);
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

        //choose which canvas essentially to draw
        //note that because its a custom gui pipeline, using unity gui terms because thats what im used to - canvas being toggleable window data without new window
        switch (currentState) {
            case states::main:
                renderMainMenuFront(gui, _anchors, window, &currentState);
                break;
            case states::optionsMenu:
                renderOptionsMenu(gui, _anchors, window, &currentState);
                break;
            case states::null:
                std::cout << "ERROR State Is Null\n";
                return -1;
        }
        

        
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
