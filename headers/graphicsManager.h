#pragma once
#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H





#include "../headers/includes.h"
#include "../headers/inputManager.h"
#include "../headers/windowManager.h"
#include "../headers/screenData.h"
#include <algorithm>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

bool imGUI = true;

glm::vec3 getStarColor(float temperature) {
    //normalize temperature (2500K-40000K)
    temperature = std::clamp(temperature, 2500.0f, 40000.0f);
    float t = (temperature - 2500.0f) / 37500.0f;

    //base colours so less range
    glm::vec3 color;
    if (t < 0.2f) {        //red
        color = glm::vec3(1.0f, 0.6f + t * 0.5f, 0.5f + t * 0.3f);
    }
    else if (t < 0.35f) {   //warm white
        color = glm::vec3(1.0f, 0.9f - (t - 0.2f) * 0.2f, 0.8f - (t - 0.2f) * 0.3f);
    }
    else if (t < 0.8f) {   //cold white
        color = glm::vec3(0.9f + (0.8f - t) * 0.2f, 0.95f, 1.0f);
    }
    else {                 //blue
        color = glm::vec3(0.7f, 0.8f, 1.0f);
    }

    //nice saturation increase
    glm::vec3 saturated = glm::mix(color, glm::normalize(color), 0.15f);

    //some stars become extra white woohoo
    if (t > 0.3f && t < 0.7f && (rand() / float(RAND_MAX)) > 0.6f) {
        return glm::vec3(1.0f);
    }

    return saturated;
}

void CreateStarfield(unsigned int& VAO, unsigned int& VBO, int starCount) {
    std::vector<float> vertices; //where all the stars are

    for (int i = 0; i < starCount; i++) {
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

        //random RGB colours
        float temperature;
        if (rand() / (float)RAND_MAX < 0.85f) {
            //85% chance: red dwarfs (2500K-4000K)
            temperature = 2500.0f + (rand() / (float)RAND_MAX) * 1500.0f;
        }
        else {
            //15% chance: hot stars (4000K-40000K)
            temperature = 4000.0f + (rand() / (float)RAND_MAX) * 36000.0f;
        }
        glm::vec3 color = getStarColor(temperature);


        //color = glm::vec3{ 1 }; //remove this for the colours to be random
        float size = 0.5f + (rand() / (float)RAND_MAX) * 2.5f;
        //add all the verticies in order so the graphics card actualy nkows what it is doing
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
        vertices.push_back(color.r);
        vertices.push_back(color.g);
        vertices.push_back(color.b);
        vertices.push_back(size);
    }

    glGenVertexArrays(1, &VAO); //bind all array objects to openGL and tell graphics card how much space
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2); 

    glBindVertexArray(0);
}

void renderImGUIWindow(GLFWwindow* window) {
    //seperate window rendering here
    glfwMakeContextCurrent(window);

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // ultra black for space
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //tell imgui new frame begin
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    float size = 1.0f;
    float color[4] = { 0.8f, 0.3f, 0.02f, 1.0f };

    ImGui::Begin("IMGUI");
    ImGui::Text("hi :)"); //text in window
    ImGui::SliderFloat("Size", &size, 0.5f, 2.0f); //slider in window
    ImGui::ColorEdit4("Color", color); //fancy colour picker
    ImGui::End(); //end window

    glfwPollEvents();
    processInput(window);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
   
    
}

void renderFrame(std::vector<std::unique_ptr<Object>>& objs, std::vector<Shader>& shaders, float deltaTime, Camera& camera, unsigned int starVAO, unsigned int starVBO, std::vector<GLFWwindow*> windows) {
    GLFWwindow* window = windows[0];
    //GLFWwindow* GUIwindow = windows[1];
    glfwMakeContextCurrent(window);
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
    glEnable(GL_PROGRAM_POINT_SIZE);
    // draw stars with no depth test
    glDisable(GL_DEPTH_TEST);
    //use the shader code
    starShader.Use();
    starShader.SetMat4("view", glm::mat4(glm::mat3(camera.GetViewMatrix())));
    starShader.SetMat4("projection", projection);
    glBindVertexArray(starVAO);
    glDrawArrays(GL_POINTS, 0, 5000); //say 5000 stars. change for fun timeeee
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
    //if (glfwWindowShouldClose(GUIwindow)) {
    //    imGUI = false;
    //}
    if (imGUI) {
        //renderImGUIWindow(GUIwindow);
        glfwMakeContextCurrent(window);
    }


    // swap front and back buffer so new screen and reset poll events. 
    glfwSwapBuffers(window);
}

#endif