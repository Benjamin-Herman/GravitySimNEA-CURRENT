#pragma once
#include "../headers/includes.h"

class Time {
public:
    // delete constructor to fix init bug
    Time() = delete;

    // static so can be called without object
    static float DeltaTime() {
        static float lastTime = glfwGetTime();
        float currentTime = glfwGetTime(); 
        float dt = currentTime - lastTime; // the instance and time between each frame calculation for dt
        lastTime = currentTime;
        return dt;
    }
};