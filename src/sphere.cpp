/*#include "../headers/sphere.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <iostream>

Sphere::Sphere(float radius, int sectors, int stacks)
    : radius(radius), sectorCount(sectors), stackCount(stacks){ //init the sphere to do its thing
    position = glm::vec3(1.0f, 1.0f, 1.0f);
    rotationAngle = 0.0f;
    vertices.clear(); //these two lines are vital as otherwise the shapers merge weirdy. some cross over data thing
    indices.clear();
    BuildVertices();
    SetupMesh();
}

void Sphere::Update(float deltaTime) {
    rotationAngle += 30.0f * deltaTime; //rotate by angle and dt
    if (rotationAngle > 360.0f) {
        rotationAngle -= 360.0f;
    }
}

glm::mat4 Sphere::GetModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position); //bunch of GLM maths stuff with pos and rot vectors
    model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    return model;
}

void Sphere::BuildVertices() { // because sphere it needs special treatment and therefore Pi
    const float PI = acos(-1.0f); //clever way of getting pi but more cpu intensive but eh

    float x, y, z, xy; //init some variables
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float s, t;

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i) { //magic maths stuff that openGL docs told me to do
        stackAngle = PI / 2 - i * stackStep;
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;

            // vertex pos
            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);

            // normalised normal, what a werid name
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;

            // vertex tex coord s and t
            s = (float)j / sectorCount;
            t = (float)i / stackCount;

            // add vertex attributes
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // colour - based on normal for nice effect
            vertices.push_back((nx + 1.0f) * 0.5f);
            vertices.push_back((ny + 1.0f) * 0.5f);
            vertices.push_back((nz + 1.0f) * 0.5f);
        }
    }

    // generate indices
    for (int i = 0; i < stackCount; ++i) { //more magic maths that openGL docs tell me to do. 
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
    glGenVertexArrays(1, &VAO); //vertex array and buffer objects, and element buffer object
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    //this is all memeory management and storing data for the grpahics card
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute assign in memory by bit and byte size. the numbers are defo important. 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Sphere::Render() {
    glBindVertexArray(VAO); //vertex array object. tell openGL to draw that stuff
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}*/