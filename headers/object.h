#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include "../headers/simData.h"

class Object {
public:
    Object(const std::string& objFilePath); 
    bool LoadFromFile(const std::string& filePath); 
    std::string getType();
    void Update(float deltaTime);
    void Render();
    glm::mat4 GetModelMatrix() const;
    ~Object() = default;
    glm::vec3 getVelocity() { return velocity; }
    glm::vec3 getAcceleration() { return acceleration; }
    glm::vec3 getPosition() { return position; }
    void setVelocity(glm::vec3 vel);
    void setAcceleration(glm::vec3 accel);
    void setPosition(glm::vec3 pos);
    float getMass() { return mass; }
    float getRadius() { return radius; }
    void setType(std::string t) { type = t; }
    void setMass(float m) { mass = m; }

protected:
    float mass = 1000000000000.0f;
    float radius = 100.0f;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    glm::vec3 position;
    float rotationAngle;
    unsigned int VAO, VBO, EBO;
    std::string type = "model";

    void SetupMesh();
};
#endif