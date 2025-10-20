#pragma once
#ifndef SIMDATA_H
#define SIMDATA_H

struct simData {
    static float gravityConstant;
    static float simSpeed;

    static float getG() { return gravityConstant; }
    static float getSpeed() { return simSpeed; }
    static void setG(float G) { gravityConstant = G; }
    static void setSpeed(float speed) { simSpeed = speed; }
};

#endif // SIMDATA_H