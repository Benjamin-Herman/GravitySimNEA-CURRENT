#pragma once
#include "../headers/includes.h"
#include "../headers/screenData.h"


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

void renderFrame(std::vector<std::unique_ptr<Object>>& objs, std::vector<Shader>& shaders, float deltaTime, Camera& camera, unsigned int starVAO, unsigned int starVBO, GLFWwindow* window) {
    Shader objectShader = shaders[0];
    Shader starShader = shaders[1];
    // clear screen. comment out for fun looking screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // ultra black for space
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // make projection so the view is like you eyes
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f), //internet maths
        (float)width / (float)height,
        0.1f,
        100.0f
    );

    // draw stars with no depth test
    glDisable(GL_DEPTH_TEST);
    //use the shader code
    starShader.Use();
    starShader.SetMat4("view", glm::mat4(glm::mat3(camera.GetViewMatrix())));
    starShader.SetMat4("projection", projection);
    glBindVertexArray(starVAO);
    glDrawArrays(GL_POINTS, 0, 5000); //say 5000 stars. Change for fun
    glEnable(GL_DEPTH_TEST); //reenable becuase depth test is kind of important

    // draw stuff
    objectShader.Use(); //use shaders and use matricies
    objectShader.SetMat4("projection", projection);
    objectShader.SetMat4("view", camera.GetViewMatrix());

    // set a light up so stuff can be seen, otherwise pretty much all black
    glm::vec3 lightPos(2.0f, 2.0f, 2.0f);
    objectShader.SetVec3("lightPos", lightPos);
    objectShader.SetVec3("viewPos", camera.Position);


    for (auto& obj : objs) {
        obj->Update(deltaTime);
        objectShader.SetMat4(obj->getType(), obj->GetModelMatrix());
        obj->Render();
    }


    // swap front and back buffer so new screen and reset poll events. 
    glfwSwapBuffers(window);
}