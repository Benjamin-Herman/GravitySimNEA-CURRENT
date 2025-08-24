#pragma once
#ifndef GRAVITY_MAIN_H
#define GRAVITY_MAIN_H

// INCLUDES
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
// END OF INCLUDES


class gravitySimMain {
public:
    
    std::vector<GLFWwindow*> windows;

    // func declarations to access anywhere in file
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

    // couple globals
    //X: 4.18292Y: 81.3964Z: -5.57994
    Camera camera;
    unsigned int starVAO, starVBO;

    int gravityMain();
    gravitySimMain() {};
private:
};

#endif
