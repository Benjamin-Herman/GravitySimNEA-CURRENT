#include "../headers/sphere.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // Add this for transformation functions
#include <cmath>
#include <iostream>

Sphere::Sphere(float radius, int sectors, int stacks)
    : radius(radius), sectorCount(sectors), stackCount(stacks),
    position(0.0f, 0.0f, 0.0f), rotationAngle(0.0f) {
    BuildVertices();
    SetupMesh();
}

void Sphere::Update(float deltaTime) {
    rotationAngle += 30.0f * deltaTime; // Rotate slower than cube
    if (rotationAngle > 360.0f) {
        rotationAngle -= 360.0f;
    }
}

glm::mat4 Sphere::GetModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    return model;
}

void Sphere::BuildVertices() {
    const float PI = acos(-1.0f);

    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float s, t;

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i) {
        stackAngle = PI / 2 - i * stackStep;
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;

            // Vertex position
            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);

            // Normalized normal
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;

            // Vertex tex coord (s, t)
            s = (float)j / sectorCount;
            t = (float)i / stackCount;

            // Add vertex attributes
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // Color (based on normal for nice effect)
            vertices.push_back((nx + 1.0f) * 0.5f);
            vertices.push_back((ny + 1.0f) * 0.5f);
            vertices.push_back((nz + 1.0f) * 0.5f);
        }
    }

    // Generate indices
    for (int i = 0; i < stackCount; ++i) {
        int k1 = i * (sectorCount + 1);
        int k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

void Sphere::SetupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Sphere::Render() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}