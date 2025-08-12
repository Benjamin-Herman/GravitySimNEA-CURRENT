#pragma once
#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include "../headers/includes.h"
#include "../headers/screenData.h"
//#include <GLFW/glfw3.h>



class windowManager {
public:
    void activateGLFW(); //actiavtes all glfw stuff
    GLFWwindow* createWindow(unsigned int iwidth = 800, unsigned int iheight = 600, const char* iscreenName = "Gravity Simulator"); //creates window

private:
    GLFWwindow* window; //window stored as pointer
};

#endif