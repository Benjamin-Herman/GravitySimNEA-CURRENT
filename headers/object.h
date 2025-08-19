#pragma once
#ifndef OBJECT_H
#define OBJECT_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>

class Object {
public:
    Object(const std::string& objFilePath); 
    bool LoadFromFile(const std::string& filePath); 
    std::string getType();
    void Update(float deltaTime);
    void Render();
    glm::mat4 GetModelMatrix() const;
    ~Object() = default;
    glm::vec3 getVelocity();
    glm::vec3 getAcceleration();
    void setVelocity(glm::vec3 vel);
    void setAcceleration(glm::vec3 accel);

protected:
    glm::vec3 velocity = glm::vec3{ 0.0f };
    glm::vec3 acceleration = glm::vec3{ 3.1f, 0.1f, 0.0f };
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    glm::vec3 position;
    float rotationAngle;
    unsigned int VAO, VBO, EBO;
    std::string type = "model";

    void SetupMesh();
};
#endif