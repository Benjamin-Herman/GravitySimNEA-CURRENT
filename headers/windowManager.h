#pragma once
#include "../headers/includes.h"
#include "../headers/screenData.h"
#include <GLFW/glfw3.h>



class windowManager {
public:
    void activateGLFW(); //actiavtes all glfw stuff
    GLFWwindow* createWindow(); //creates window

private:
    GLFWwindow* window; //window stored as pointer
};