#include "../headers/inputManager.h"


//globals for mouse tracking
float lastX = screenData::width / 2.0f; //middle of the screen
float lastY = screenData::height / 2.0f;
bool firstMouse = true;
bool mouseDisabled = false;


//arrow key rotate keybind
void inputManager::processArrowKeys(GLFWwindow* window, float deltaTime, Camera& camera) {
    float sensitivity = 50.0f * deltaTime; //trial and error got to 50

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) { //if left key then rotate left
        camera.ProcessArrowKeys(-sensitivity, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) { //if right key then rotate right
        camera.ProcessArrowKeys(sensitivity, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) { //if up key then rotate up
        camera.ProcessArrowKeys(0.0f, sensitivity);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) { //if down key then rotate down
        camera.ProcessArrowKeys(0.0f, -sensitivity);
    }
}

//check ESC and mouse button input
void inputManager::processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { //check escape input
        //glfwSetWindowShouldClose(window, true); //close window
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstMouse = true;
        mouseDisabled = true;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) { //if left mouse pressed
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        mouseDisabled = false;
    }
}

//process mouse movement
void inputManager::mouseInput(GLFWwindow* window, double xpos, double ypos, Camera& camera) {
    if (mouseDisabled) { //ignore input if mouse disabled
        return;
    }
    if (firstMouse) { //initial mouse positioning
        lastX = xpos;
        lastY = ypos; //set to centre screen on last x and y to ensure no null values
        firstMouse = false;
    }

    float xoffset = xpos - lastX; //get offset from last position and process movement by how much offset
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset); //apply movement
}
