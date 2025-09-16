#pragma once
#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "../headers/screenData.h"
#include "../headers/camera.h"
#include "../headers/gravityMain.h"
#include <GLFW/glfw3.h>
class gravityMain;
class inputManager {
public://func declarations
	void processArrowKeys(GLFWwindow * window, float deltaTime, Camera & camera); //arrow key rotate keybind
	void processInput(GLFWwindow* window, gravityMain* gMain); //check ESC and mouse button input
	void mouseInput(GLFWwindow * window, double xpos, double ypos, Camera & camera); //process mouse movement
};




#endif
