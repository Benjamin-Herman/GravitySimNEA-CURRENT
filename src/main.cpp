#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <vector>


#include "../headers/includes.h"
#include "../headers/cube.h"
#include "../headers/sphere.h"
#include "../headers/shader.h"
#include "../headers/camera.h"
#include "../headers/graphicsManager.h"
#include "../headers/inputManager.h"
//#include "../headers/screenData.h"
#include "../headers/time.h"
//#include "../headers/object.h"
#include "../headers/windowManager.h"




std::vector<GLFWwindow*> windows;

// func declarations to access anywhere in file
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// couple globals
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
unsigned int starVAO, starVBO;

int main() {
    //random seed for the stars. no clue why its still in main.cpp
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    windowManager wm; //create windowmanager instance. activate GLFW and create window
    wm.activateGLFW();
    GLFWwindow* window = wm.createWindow();
    //GLFWwindow* GUIwindow = wm.createWindow(220, 220, "UI WINDOW", window, true);
    windows.push_back(window);
    //windows.push_back(GUIwindow);

    // input callback thingys
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //GUI window callback things
    //glfwSetCursorPosCallback(GUIwindow, mouse_callback);
    //glfwSetInputMode(GUIwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    std::vector<Object> objs;
    Object sphere("models/sphere.obj");
    Object plane("models/plane.obj");
    objs.push_back(sphere);
    objs.push_back(plane);

    // render loop. TODO move to graphics manager
    while (!glfwWindowShouldClose(window)/* || !glfwWindowShouldClose(GUIwindow)*/) {
        float deltaTime = Time::DeltaTime(); //get delta time
        //process input commands. basically say i call youuu
        wm.showFPS();
        processInput(window);
        processArrowKeys(window, deltaTime, camera);
        camera.ProcessKeyboard(window, deltaTime);

        renderFrame(objs, shaders, deltaTime, camera, starVAO, starVBO, windows);
        objs[1].setVelocity(glm::vec3{ 0 });
        glfwPollEvents();
    }

    // memeory clean becuase cpp sucks
    glDeleteVertexArrays(1, &starVAO);
    glDeleteBuffers(1, &starVBO);
    glfwTerminate();

    /*wm.activateGLFW();
    GLFWwindow* newWindow = wm.createWindow(400, 400, "BYE BYE");
    while (!glfwWindowShouldClose(newWindow)) {
        glfwPollEvents();
    }
    glfwTerminate();/**/

    return 0;
}

//must be in here much to my dismay as otherwise i dont get helper stuff and i dunno how that stuff works to manipulate
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    mouseInput(window, xpos, ypos, camera); //call the function elsewhere. needed this for openGL cheats
}