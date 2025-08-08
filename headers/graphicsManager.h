#pragma once
#include "../headers/includes.h"

void CreateStarfield(unsigned int& VAO, unsigned int& VBO, int starCount) {
    std::vector<float> vertices; //where all the stars are

    for (int i = 0; i < starCount; ++i) {
        //choose a random angle and place to put on the edge
        float theta = 2.0f * glm::pi<float>() * (rand() / (float)RAND_MAX);
        float phi = acos(1.0f - 2.0f * (rand() / (float)RAND_MAX));
        float r = 50.0f;

        //fancy maths i got literally no idea why this works
        float x = r * sin(phi) * cos(theta);
        float y = r * sin(phi) * sin(theta);
        float z = r * cos(phi);
        //random brightness
        float brightness = 0.5f + (rand() / (float)RAND_MAX) * 0.5f;
        //add all the verticies in order so the graphics card actualy nkows what it is doing
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
        vertices.push_back(brightness);
        vertices.push_back(brightness);
        vertices.push_back(brightness);
    }

    glGenVertexArrays(1, &VAO); //bind all array objects to openGL and tell graphics card how much space
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void renderFrame() {

}