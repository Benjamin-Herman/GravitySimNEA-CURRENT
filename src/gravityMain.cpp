#include "../headers/gravityMain.h"



int gravityMain::gravitySimMain() {
    // random seed for the stars
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    windowManager wm; // create windowmanager instance. activate GLFW and create window
    wm.activateGLFW();
    GLFWwindow* window = wm.createWindow(800, 600);
    windows.push_back(window);

    // store this instance for use in the static callback
    glfwSetWindowUserPointer(window, this);

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

    saveLoader saveManager;
    std::vector<Object> objs = saveManager.loadSave("saves/test.save", camera); // calls the savemanager to load scene
    //std::cout << objs.size();

    // render loop. TODO move to graphics manager
    while (!glfwWindowShouldClose(window)/* || !glfwWindowShouldClose(GUIwindow)*/) {
        float deltaTime = Time::DeltaTime(); // get delta time
        // process input commands. basically say i call youuu
        wm.showFPS();
        processInput(window);
        processArrowKeys(window, deltaTime, camera);
        camera.ProcessKeyboard(window, deltaTime);
        //std::cout << "X: " << camera.Position.x << "Y: " << camera.Position.y << "Z: " << camera.Position.z << "\n";
        gravitySystemUpdate(objs);
        renderFrame(objs, shaders, deltaTime, camera, starVAO, starVBO, windows);
        glfwPollEvents();
    }

    // memeory clean becuase cpp sucks
    glDeleteVertexArrays(1, &starVAO);
    glDeleteBuffers(1, &starVBO);
    glfwTerminate();

    return 0;
}


void gravityMain::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    //get class instance from user pointer - basically make sure i call function on the right part of memeory
    auto* self = static_cast<gravityMain*>(glfwGetWindowUserPointer(window));
    if (self) {
        // call the function elsewhere. needed this for openGL cheats
        mouseInput(window, xpos, ypos, self->camera);
    }
}