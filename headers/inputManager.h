#pragma once
#include "../headers/screenData.h"

float lastX = width / 2.0f;
float lastY = height / 2.0f; //middle of the screen
bool firstMouse = true;

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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { //closes window on escape pressed
        glfwSetWindowShouldClose(window, true);
    }
}

void mouseInput(GLFWwindow* window, double xpos, double ypos, Camera& camera) {
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

