#pragma once

#ifndef CAMERA_H
#define CAMERA_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f));

    void ProcessKeyboard(GLFWwindow* window, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset);
    void ProcessArrowKeys(float xoffset, float yoffset);
    glm::mat4 GetViewMatrix() const;

    // Public variables for easy access
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Yaw;
    float Pitch;
    float HeightSpeed;

private:
    void updateCameraVectors();
};

#endif