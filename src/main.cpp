#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <vector>

#include "../headers/gravityManager.h"
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
#include "../headers/saveLoader.h"

std::vector<GLFWwindow*> windows;

// func declarations to access anywhere in file
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// couple globals
//X: 4.18292Y: 81.3964Z: -5.57994
Camera camera(glm::vec3(4.0f, 80.0f, 5.0f), -85.0f, 45.0f);
unsigned int starVAO, starVBO;

int main() {
    //random seed for the stars. no clue why its still in main.cpp
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    windowManager wm; //create windowmanager instance. activate GLFW and create window
    wm.activateGLFW();
    GLFWwindow* window = wm.createWindow(800, 600);
    windows.push_back(window);

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
    std::vector<Object> objs = saveManager.loadSave("saves/test.save", camera); //calls the savemanager to load scene
    //std::cout << objs.size();

    // render loop. TODO move to graphics manager
    while (!glfwWindowShouldClose(window)/* || !glfwWindowShouldClose(GUIwindow)*/) {
        float deltaTime = Time::DeltaTime(); //get delta time
        //process input commands. basically say i call youuu
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

//must be in here much to my dismay as otherwise i dont get helper stuff and i dunno how that stuff works to manipulate
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    mouseInput(window, xpos, ypos, camera); //call the function elsewhere. needed this for openGL cheats
}