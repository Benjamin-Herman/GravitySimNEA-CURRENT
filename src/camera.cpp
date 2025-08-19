#include "../headers/camera.h"

Camera::Camera(glm::vec3 position) //init object 
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
    MovementSpeed(50.0f), //set variables up for movement etc
    MouseSensitivity(0.1f),
    HeightSpeed(30.0f),
    Yaw(-90.0f),
    Pitch(0.0f) {
    Position = position;
    WorldUp = glm::vec3(0.0f, 1.0f, 0.0f); //a relative vector for maths stuff
    updateCameraVectors();
}

void Camera::ProcessArrowKeys(float xoffset, float yoffset) {
    xoffset *= MouseSensitivity * 5.0f; // slower that mouse movement for precision
    yoffset *= MouseSensitivity * 5.0f;

    Yaw += xoffset;
    Pitch += yoffset;

    // constrain to prevent spinning full 360 up pitch
    if (Pitch > 89.0f)
        Pitch = 89.0f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;

    updateCameraVectors();
}


void Camera::ProcessKeyboard(GLFWwindow* window, float deltaTime) {
    if (window == nullptr) return;

    float dt = Time::DeltaTime();

    float velocity = MovementSpeed * deltaTime;
    float heightVelocity = HeightSpeed * deltaTime;
    glm::vec3 moveDir(0.0f);

    //handle WASD movement. 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        moveDir += Front * dt; //these are all vector directions. make maths easier i think
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        moveDir -= Front * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        moveDir -= Right * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        moveDir += Right * dt;
    }

    //handle up and down with EQ keys
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        moveDir += Up * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        moveDir -= Up * dt;
    }

    //normalise movement vectors so lengeth = 1. some maths thing which make comparisons easy :)
    if (glm::length(moveDir) > 0.0f) {
        moveDir = glm::normalize(moveDir);
    }

    //tell position vector to add whatever the speed is and direction. fancy maths stuff handled by glut
    Position += moveDir * velocity;
    //std::cout << "CAMERA: " << Position.x << " " << Position.y << " " << Position.z << "\n";
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset) {
    xoffset *= MouseSensitivity; //basically say move faster
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // same stuff that was in the arrow keys
    if (Pitch > 89.0f) {
        Pitch = 89.0f;
    }
    if (Pitch < -89.0f) {
        Pitch = -89.0f;
    }

    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(Position, Position + Front, Up); //some documentation said this. it gets the view matrix
}

void Camera::updateCameraVectors() {
    // calc new front facing vector
    glm::vec3 front; //honesty no idea what this does i just know google says thats how i turn
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    // recalc up and right vectors
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}