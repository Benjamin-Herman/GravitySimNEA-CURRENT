#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Object {
public:
    Object();
    virtual void Update(float deltaTime);
    virtual void Render();
    virtual glm::mat4 GetModelMatrix() const;
    virtual ~Object() = default;

protected:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    glm::vec3 position;
    float rotationAngle;
    unsigned int VAO, VBO, EBO;

    virtual void SetupMesh();
};