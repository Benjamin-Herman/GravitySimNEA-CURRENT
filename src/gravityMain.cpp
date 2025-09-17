#include "../headers/gravityMain.h"


//windowManager window_Manager; // create windowindow_Manageranager instance. 
graphicsManager graphics_Manager;
physicsManager physics_Manager;
inputManager input_Manager;
saveLoader saveManager;

void gravityMain::execute(){
    std::vector<Object> objs = saveManager.loadSave("saves/test.save", camera); // calls the savemanager to load scene
    //std::cout << objs.size();
    // render loop. TODO move to graphics manager
    while (!glfwWindowShouldClose(window)/* || !glfwWindowShouldClose(GUIwindow)*/) {
        float deltaTime = Time::DeltaTime(); // get delta time
        // process input commands. basically say i call youuu
        window_Manager.showFPS();
        input_Manager.processInput(window, this);
        input_Manager.processArrowKeys(window, deltaTime, camera);
        camera.ProcessKeyboard(window, deltaTime);
        //std::cout << "X: " << camera.Position.x << "Y: " << camera.Position.y << "Z: " << camera.Position.z << "\n";
        physics_Manager.gravitySystemUpdate(objs);
        graphics_Manager.renderFrame(objs, shaders, deltaTime, camera, starVAO, starVBO, windows);
        glfwPollEvents();
       
    }


    
}

int gravityMain::gravitySimMain() {
    // random seed for the stars
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    //activate GLFW and create window
    window_Manager.activateGLFW();
    window = window_Manager.createWindow(800, 600);
    windows.push_back(window);

    // store this instance for use in the static callback
    glfwSetWindowUserPointer(window, this);

    // input callback thingys
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);




    // enable depth testing
    glEnable(GL_DEPTH_TEST);

    // compile shaders
    Shader objectShader("shaders/shader.vert", "shaders/shader.frag");
    Shader starShader("shaders/starShader.vert", "shaders/starShader.frag");
    shaders.push_back(objectShader);
    shaders.push_back(starShader);

    // make starsssss

    graphics_Manager.CreateStarfield(starVAO, starVBO, 5000);
    //-------------------------------------------------

    execute();
    
    // memeory clean becuase cpp sucks
    glDeleteVertexArrays(1, &starVAO);
    glDeleteBuffers(1, &starVBO);
    //glfwTerminate();
    return 0;
}

int gravityMain::restart()
{
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    glfwTerminate();

    shaders.clear();             
    windows.clear();
    

    gravitySimMain(); 
    return 0;
}



void gravityMain::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    //get class instance from user pointer - basically make sure i call function on the right part of memeory
    auto* self = static_cast<gravityMain*>(glfwGetWindowUserPointer(window));
    if (self) {
        //call the function elsewhere. needed this for openGL cheats
        input_Manager.mouseInput(window, xpos, ypos, self->camera);
    }
}