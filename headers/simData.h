#pragma once
#ifndef SIMDATA_H
#define SIMDATA_H

struct simData {
    static float gravityConstant;
    static float simSpeed;
    static float lightSpeed;

    static float getG() { return gravityConstant; }
    static float getSpeed() { return simSpeed; }
    static float getLightSpeed() { return lightSpeed; }
    static void setG(float G) { gravityConstant = G; }
    static void setSpeed(float speed) { simSpeed = speed; }
    static void setLightSpeed(float speed) { lightSpeed = speed; }
};

#endif // SIMDATA_H