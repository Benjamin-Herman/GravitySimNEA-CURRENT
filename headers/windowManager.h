#pragma once
#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include "../headers/includes.h"
//#include "../headers/screenData.h"
#include <chrono>
//#include <GLFW/glfw3.h>



class windowManager {
public:
    void activateGLFW(); //actiavtes all glfw stuff
    void showFPS();
    GLFWwindow* createWindow(unsigned int width = 800, unsigned int height = 600,
        const char* screenName = "Graphics Simulator",
        GLFWwindow* share = nullptr, bool isGUI = false);
private:
    GLFWwindow* window; //window stored as pointer
    std::string name;
};

extern class windowManager window_Manager;

#endif