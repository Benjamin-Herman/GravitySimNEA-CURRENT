#pragma once
#ifndef TIME_H
#define TIME_H


#include "../headers/includes.h"
class Time {
public:
    // delete constructor to fix init bug
    Time() = delete;
    // static so can be called without object
    static float DeltaTime() {
        static double lastTime = glfwGetTime();  //keep full precision
        double currentTime = glfwGetTime();
        double dt = currentTime - lastTime; // the instance and time between each frame calculation for dt
        lastTime = currentTime;

        return static_cast<float>(dt); //return as float
    }
};
#endif