#pragma once
#ifndef CUBE_H
#define CUBE_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "../headers/object.h"

class Cube : public Object {

};

/*class Cube {
public:
    Cube();
    void Update(float deltaTime);
    void Render();
    glm::mat4 GetModelMatrix() const;

private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    glm::vec3 position;
    float rotationAngle;
    unsigned int VAO, VBO, EBO;

    void SetupMesh();
};*/

#endif