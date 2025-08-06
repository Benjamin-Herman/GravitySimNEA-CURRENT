#include "../headers/camera.h"

Camera::Camera(glm::vec3 position)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
    MovementSpeed(5.0f),
    MouseSensitivity(0.1f),
    HeightSpeed(3.0f),
    Yaw(-90.0f),
    Pitch(0.0f) {
    Position = position;
    WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    updateCameraVectors();
}

void Camera::ProcessArrowKeys(float xoffset, float yoffset) {
    xoffset *= MouseSensitivity * 5.0f; // Slower than mouse for better control
    yoffset *= MouseSensitivity * 5.0f;

    Yaw += xoffset;
    Pitch += yoffset;

    // Constrain pitch to prevent flipping
    if (Pitch > 89.0f)
        Pitch = 89.0f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;

    updateCameraVectors();
}


void Camera::ProcessKeyboard(GLFWwindow* window, float deltaTime) {
    if (window == nullptr) return;

    float velocity = MovementSpeed * deltaTime;
    float heightVelocity = HeightSpeed * deltaTime;
    glm::vec3 moveDir(0.0f);

    // Horizontal movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        moveDir += Front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        moveDir -= Front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        moveDir -= Right;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        moveDir += Right;

    // Vertical movement
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        Position.y += heightVelocity;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        Position.y -= heightVelocity;

    // Normalize horizontal movement if needed
    if (glm::length(moveDir) > 0.0f)
        moveDir = glm::normalize(moveDir);

    // Apply horizontal movement
    Position += moveDir * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // Constrain pitch to prevent flipping
    if (Pitch > 89.0f)
        Pitch = 89.0f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;

    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::updateCameraVectors() {
    // Calculate new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    // Re-calculate Right and Up vectors
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}