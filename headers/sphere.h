#pragma once

#ifndef SPHERE_H
#define SPHERE_H


#include <glm/glm.hpp>
#include <vector>

#include "../headers/object.h"

class Sphere : public Object {
public:
    Sphere(float radius = 1.0f, int sectors = 36, int stacks = 18);
    void Render() override;
    glm::mat4 GetModelMatrix() const override;
    void Update(float deltaTime) override;
        
private:
    void BuildVertices(); 
    void SetupMesh() override;
    float radius;
    int sectorCount;
    int stackCount;
};

#endif