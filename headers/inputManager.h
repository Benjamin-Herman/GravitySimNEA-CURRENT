#pragma once

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H


#include "../headers/screenData.h"

float lastX = width / 2.0f;
float lastY = height / 2.0f; //middle of the screen
bool firstMouse = true;
bool mouseDisabled = false;

void processArrowKeys(GLFWwindow* window, float deltaTime, Camera& camera) { //arror key rotate keybind
    float sensitivity = 50.0f * deltaTime; //trial and error got to 50

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) { //if whatever key then call process with correct val
        camera.ProcessArrowKeys(-sensitivity, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        camera.ProcessArrowKeys(sensitivity, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        camera.ProcessArrowKeys(0.0f, sensitivity);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        camera.ProcessArrowKeys(0.0f, -sensitivity);
    }
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { //check escape input
        //glfwSetWindowShouldClose(window, true); //close window
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstMouse = true;
        mouseDisabled = true;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        mouseDisabled = false;
    }
}

void mouseInput(GLFWwindow* window, double xpos, double ypos, Camera& camera) {
    if (mouseDisabled) {
        return;
    }
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos; //set to centre screen on last x and y to ensure no null values
        firstMouse = false;
    }

    float xoffset = xpos - lastX; //get offset from last position and process movement by how much offset
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


#endif