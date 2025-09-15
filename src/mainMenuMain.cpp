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

    button loginBtn(&gui);
    loginBtn.setOnClick([_currentState]() {
        *_currentState = mainMenuClass::states::loginMenu;
        });

    button saveLoadBtn(&gui);
    saveLoadBtn.setOnClick([_currentState]() {
        *_currentState = mainMenuClass::states::saveLoadMenu;
        });

    button quitBtn(&gui);
    quitBtn.setOnClick([window]() {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        });

    //Calculate dynamic scaling based on window size
    float scaleFactor = std::min(_anchors.screenWidth / 800.0f, _anchors.screenHeight / 600.0f);
    float titleScale = 1.0f * scaleFactor;
    float buttonScale = 0.8f * scaleFactor;

    glm::vec2 titleOffset = glm::vec2(0.0f, -_anchors.screenHeight * 0.15f);
    glm::vec2 buttonSpacing = glm::vec2(0.0f, _anchors.screenHeight * 0.12f);
    glm::vec2 buttonSize = glm::vec2(_anchors.screenWidth * 0.4f, _anchors.screenHeight * 0.1f);
    glm::vec2 textOffset = glm::vec2(buttonSize.x * 0.05f, buttonSize.y * 0.4f);

    //TITLE TEXT
    gui.renderText("MAIN MENU", _anchors.topMiddle + titleOffset, titleScale, glm::vec3{ 1.f });

    //BUTTONS
    beginBtn.renderButton(
        "BEGIN",
        _anchors.middleMiddle - buttonSpacing * 2.0f,
        buttonScale,
        glm::vec3{ 1.f },
        buttonSize,
        glm::vec3{ 1.f, 0.f, 0.f },
        textOffset
    );

    optionsBtn.renderButton(
        "OPTIONS",
        _anchors.middleMiddle - buttonSpacing * 1.0f,
        buttonScale,
        glm::vec3{ 1.f },
        buttonSize,
        glm::vec3{ 1.f, 0.f, 0.f },
        textOffset
    );

    loginBtn.renderButton(
        "LOGIN/SIGNUP",
        _anchors.middleMiddle,
        buttonScale,
        glm::vec3{ 1.f },
        buttonSize,
        glm::vec3{ 1.f, 0.f, 0.f },
        textOffset
    );

    saveLoadBtn.renderButton(
        "LOAD GAME",
        _anchors.middleMiddle + buttonSpacing * 1.0f,
        buttonScale,
        glm::vec3{ 1.f },
        buttonSize,
        glm::vec3{ 1.f, 0.f, 0.f },
        textOffset
    );

    quitBtn.renderButton(
        "QUIT",
        _anchors.middleMiddle + buttonSpacing * 2.0f,
        buttonScale,
        glm::vec3{ 1.f },
        buttonSize,
        glm::vec3{ 1.f, 0.f, 0.f },
        textOffset
    );
}

void renderOptionsMenu(GUI& gui, const GUI::anchors& _anchors, GLFWwindow* window, mainMenuClass::states* _currentState) {
    //create buttons
    button backBtn(&gui);
    backBtn.setOnClick([_currentState]() {
        *_currentState = mainMenuClass::states::main;
        });

    //Create unique sliders with different IDs
    static slider volumeSlider(&gui, "volume");
    static slider brightnessSlider(&gui, "brightness");
    static slider sensitivitySlider(&gui, "sensitivity");

    //Calculate dynamic scaling
    float scaleFactor = std::min(_anchors.screenWidth / 800.0f, _anchors.screenHeight / 600.0f);
    float titleScale = 1.0f * scaleFactor;
    float textScale = 0.7f * scaleFactor;

    glm::vec2 titleOffset = glm::vec2(0.0f, -_anchors.screenHeight * 0.2f);
    glm::vec2 sliderSpacing = glm::vec2(0.0f, _anchors.screenHeight * 0.1f);
    glm::vec2 sliderSize = glm::vec2(_anchors.screenWidth * 0.4f, _anchors.screenHeight * 0.03f);
    glm::vec2 buttonSize = glm::vec2(_anchors.screenWidth * 0.4f, _anchors.screenHeight * 0.1f);
    glm::vec2 textOffset = glm::vec2(buttonSize.x * 0.05f, buttonSize.y * 0.4f);
    glm::vec2 labelOffset = glm::vec2(-_anchors.screenWidth * 0.22f, 0.0f);

    //TITLE TEXT
    gui.renderText("OPTIONS MENU", _anchors.topMiddle + titleOffset, titleScale, glm::vec3{ 1.f });

    //VOLUME SLIDER
    gui.renderText("Volume", _anchors.middleMiddle - sliderSpacing * 2.0f + labelOffset, textScale, glm::vec3{ 1.f });
    volumeSlider.renderSlider(
        "",
        _anchors.middleMiddle - sliderSpacing * 2.0f,
        textScale,
        glm::vec3{ 1.f },
        sliderSize,
        glm::vec3{ 0.9f, 0.9f, 0.9f },
        glm::vec2{ 10.f, 50.f },
        sliderSize.y * 0.8f
    );

    //BRIGHTNESS SLIDER
    gui.renderText("Brightness", _anchors.middleMiddle - sliderSpacing * 1.0f + labelOffset, textScale, glm::vec3{ 1.f });
    brightnessSlider.renderSlider(
        "",
        _anchors.middleMiddle - sliderSpacing * 1.0f,
        textScale,
        glm::vec3{ 1.f },
        sliderSize,
        glm::vec3{ 0.9f, 0.9f, 0.9f },
        glm::vec2{ 10.f, 50.f },
        sliderSize.y * 0.8f
    );

    //SENSITIVITY SLIDER
    gui.renderText("Sensitivity", _anchors.middleMiddle + labelOffset, textScale, glm::vec3{ 1.f });
    sensitivitySlider.renderSlider(
        "",
        _anchors.middleMiddle,
        textScale,
        glm::vec3{ 1.f },
        sliderSize,
        glm::vec3{ 0.9f, 0.9f, 0.9f },
        glm::vec2{ 10.f, 50.f },
        sliderSize.y * 0.8f
    );

    backBtn.renderButton(
        "BACK",
        _anchors.middleMiddle + sliderSpacing * 2.0f,
        textScale * 1.2f,
        glm::vec3{ 1.f },
        buttonSize,
        glm::vec3{ 1.f, 0.f, 0.f },
        textOffset
    );
}

void renderLoginMenu(GUI& gui, const GUI::anchors& _anchors, GLFWwindow* window, mainMenuClass::states* _currentState) {
    button backBtn(&gui);
    backBtn.setOnClick([_currentState]() {
        *_currentState = mainMenuClass::states::main;
        });

    button loginBtn(&gui);
    loginBtn.setOnClick([]() {
        std::cout << "Login attempted\n";
        });

    button signupBtn(&gui);
    signupBtn.setOnClick([]() {
        std::cout << "Signup attempted\n";
        });

    //Calculate dynamic scaling
    float scaleFactor = std::min(_anchors.screenWidth / 800.0f, _anchors.screenHeight / 600.0f);
    float titleScale = 1.0f * scaleFactor;
    float textScale = 0.7f * scaleFactor;

    glm::vec2 titleOffset = glm::vec2(0.0f, -_anchors.screenHeight * 0.2f);
    glm::vec2 fieldSpacing = glm::vec2(0.0f, _anchors.screenHeight * 0.1f);
    glm::vec2 fieldSize = glm::vec2(_anchors.screenWidth * 0.4f, _anchors.screenHeight * 0.06f);
    glm::vec2 buttonSize = glm::vec2(_anchors.screenWidth * 0.4f, _anchors.screenHeight * 0.1f);
    glm::vec2 textOffset = glm::vec2(buttonSize.x * 0.05f, buttonSize.y * 0.4f);
    glm::vec2 labelOffset = glm::vec2(-_anchors.screenWidth * 0.22f, 0.0f);

    //TITLE TEXT
    gui.renderText("LOGIN/SIGNUP", _anchors.topMiddle + titleOffset, titleScale, glm::vec3{ 1.f });

    //USERNAME FIELD
    gui.renderText("Username:", _anchors.middleMiddle - fieldSpacing * 1.5f + labelOffset, textScale, glm::vec3{ 1.f });
    gui.renderShape(_anchors.middleMiddle - fieldSpacing * 1.5f - glm::vec2(fieldSize.x / 2, fieldSize.y / 2),
        fieldSize, glm::vec3{ 0.2f, 0.2f, 0.2f }, "rectangle");

    //PASSWORD FIELD
    gui.renderText("Password:", _anchors.middleMiddle - fieldSpacing * 0.5f + labelOffset, textScale, glm::vec3{ 1.f });
    gui.renderShape(_anchors.middleMiddle - fieldSpacing * 0.5f - glm::vec2(fieldSize.x / 2, fieldSize.y / 2),
        fieldSize, glm::vec3{ 0.2f, 0.2f, 0.2f }, "rectangle");

    loginBtn.renderButton(
        "LOGIN",
        _anchors.middleMiddle + fieldSpacing * 0.5f - glm::vec2(buttonSize.x / 2, 0),
        textScale * 1.2f,
        glm::vec3{ 1.f },
        buttonSize,
        glm::vec3{ 0.f, 1.f, 0.f },
        textOffset
    );

    signupBtn.renderButton(
        "SIGN UP",
        _anchors.middleMiddle + fieldSpacing * 1.5f - glm::vec2(buttonSize.x / 2, 0),
        textScale * 1.2f,
        glm::vec3{ 1.f },
        buttonSize,
        glm::vec3{ 0.f, 0.f, 1.f },
        textOffset
    );

    backBtn.renderButton(
        "BACK",
        _anchors.middleMiddle + fieldSpacing * 2.5f - glm::vec2(buttonSize.x / 2, 0),
        textScale * 1.2f,
        glm::vec3{ 1.f },
        buttonSize,
        glm::vec3{ 1.f, 0.f, 0.f },
        textOffset
    );
}

void renderSaveLoadMenu(GUI& gui, const GUI::anchors& _anchors, GLFWwindow* window, mainMenuClass::states* _currentState) {
    button backBtn(&gui);
    backBtn.setOnClick([_currentState]() {
        *_currentState = mainMenuClass::states::main;
        });

    //Sample save file buttons
    button saveBtn1(&gui);
    saveBtn1.setOnClick([]() {
        std::cout << "Loading: Solar System Simulation\n";
        });

    button saveBtn2(&gui);
    saveBtn2.setOnClick([]() {
        std::cout << "Loading: Galaxy Collision\n";
        });

    button saveBtn3(&gui);
    saveBtn3.setOnClick([]() {
        std::cout << "Loading: Black Hole Test\n";
        });

    button saveBtn4(&gui);
    saveBtn4.setOnClick([]() {
        std::cout << "Loading: Custom Simulation 1\n";
        });

    //Calculate dynamic scaling
    float scaleFactor = std::min(_anchors.screenWidth / 800.0f, _anchors.screenHeight / 600.0f);
    float titleScale = 1.0f * scaleFactor;
    float textScale = 0.6f * scaleFactor;
    float smallTextScale = 0.5f * scaleFactor;

    glm::vec2 titleOffset = glm::vec2(0.0f, -_anchors.screenHeight * 0.2f);
    glm::vec2 saveSpacing = glm::vec2(0.0f, _anchors.screenHeight * 0.12f);
    glm::vec2 saveSize = glm::vec2(_anchors.screenWidth * 0.6f, _anchors.screenHeight * 0.1f);
    glm::vec2 buttonSize = glm::vec2(_anchors.screenWidth * 0.4f, _anchors.screenHeight * 0.1f);
    glm::vec2 textOffset = glm::vec2(saveSize.x * 0.02f, saveSize.y * 0.3f);
    glm::vec2 dateOffset = glm::vec2(saveSize.x * 0.02f, saveSize.y * 0.7f);

    //TITLE TEXT
    gui.renderText("LOAD GAME", _anchors.topMiddle + titleOffset, titleScale, glm::vec3{ 1.f });

    //SAVE FILE LIST
    gui.renderText("AVAILABLE SAVES", _anchors.middleMiddle - saveSpacing * 3.5f, textScale * 1.1f, glm::vec3{ 0.f, 1.f, 1.f });

    //Save file 1
    saveBtn1.renderButton(
        "Solar System Simulation",
        _anchors.middleMiddle - saveSpacing * 2.5f - glm::vec2(saveSize.x / 2, 0),
        textScale,
        glm::vec3{ 1.f },
        saveSize,
        glm::vec3{ 0.2f, 0.2f, 0.3f },
        textOffset
    );
    gui.renderText("Created: 2023-12-15 14:30 | Saved: 2023-12-15 16:45",
        _anchors.middleMiddle - saveSpacing * 2.5f - glm::vec2(saveSize.x / 2, 0) + dateOffset,
        smallTextScale, glm::vec3{ 0.8f, 0.8f, 0.8f });

    //Save file 2
    saveBtn2.renderButton(
        "Galaxy Collision",
        _anchors.middleMiddle - saveSpacing * 1.5f - glm::vec2(saveSize.x / 2, 0),
        textScale,
        glm::vec3{ 1.f },
        saveSize,
        glm::vec3{ 0.2f, 0.3f, 0.2f },
        textOffset
    );
    gui.renderText("Created: 2023-12-14 10:15 | Saved: 2023-12-14 12:30",
        _anchors.middleMiddle - saveSpacing * 1.5f - glm::vec2(saveSize.x / 2, 0) + dateOffset,
        smallTextScale, glm::vec3{ 0.8f, 0.8f, 0.8f });

    //Save file 3
    saveBtn3.renderButton(
        "Black Hole Test",
        _anchors.middleMiddle - saveSpacing * 0.5f - glm::vec2(saveSize.x / 2, 0),
        textScale,
        glm::vec3{ 1.f },
        saveSize,
        glm::vec3{ 0.3f, 0.2f, 0.2f },
        textOffset
    );
    gui.renderText("Created: 2023-12-13 09:00 | Saved: 2023-12-13 11:20",
        _anchors.middleMiddle - saveSpacing * 0.5f - glm::vec2(saveSize.x / 2, 0) + dateOffset,
        smallTextScale, glm::vec3{ 0.8f, 0.8f, 0.8f });

    //Save file 4
    saveBtn4.renderButton(
        "Custom Simulation 1",
        _anchors.middleMiddle + saveSpacing * 0.5f - glm::vec2(saveSize.x / 2, 0),
        textScale,
        glm::vec3{ 1.f },
        saveSize,
        glm::vec3{ 0.3f, 0.3f, 0.2f },
        textOffset
    );
    gui.renderText("Created: 2023-12-12 16:45 | Saved: 2023-12-12 18:15",
        _anchors.middleMiddle + saveSpacing * 0.5f - glm::vec2(saveSize.x / 2, 0) + dateOffset,
        smallTextScale, glm::vec3{ 0.8f, 0.8f, 0.8f });

    backBtn.renderButton(
        "BACK",
        _anchors.middleMiddle + saveSpacing * 2.0f - glm::vec2(buttonSize.x / 2, 0),
        textScale * 1.2f,
        glm::vec3{ 1.f },
        buttonSize,
        glm::vec3{ 1.f, 0.f, 0.f },
        textOffset
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
        case states::loginMenu:
            renderLoginMenu(gui, _anchors, window, &currentState);
            break;
        case states::saveLoadMenu:
            renderSaveLoadMenu(gui, _anchors, window, &currentState);
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