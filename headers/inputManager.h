#pragma once
#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "../headers/screenData.h"
#include "../headers/camera.h"
#include <GLFW/glfw3.h>


//globals for mouse tracking
extern float lastX; //middle of the screen
extern float lastY;
extern bool firstMouse;
extern bool mouseDisabled;

//func declarations
void processArrowKeys(GLFWwindow* window, float deltaTime, Camera& camera); //arrow key rotate keybind
void processInput(GLFWwindow* window); //check ESC and mouse button input
void mouseInput(GLFWwindow* window, double xpos, double ypos, Camera& camera); //process mouse movement

#endif
