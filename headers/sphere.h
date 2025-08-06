#pragma once
#include <glm/glm.hpp>
#include <vector>

class Sphere {
public:
    Sphere(float radius = 1.0f, int sectors = 36, int stacks = 18);
    void Update(float deltaTime);
    void Render();
    glm::mat4 GetModelMatrix() const;

private:
    void SetupMesh();
    void BuildVertices();

    float radius;
    int sectorCount;
    int stackCount;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 position;
    float rotationAngle;

    unsigned int VAO, VBO, EBO;
};