#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "stb_truetype.h"
#include "shader.h"
#include <glm/glm.hpp>
#include <functional> // for button callbacks

class GUI {
public:
    GUI(GLFWwindow* win); //creates the GUI with whichever window it wants to be in
    ~GUI();






    bool loadFont(const char* ttf_path, float pixel_height);
    void renderText(const std::string& text, glm::vec2 coord, float scale, glm::vec3 colour);
    void updateSize(GLFWwindow* window);
    void renderShape(glm::vec2 coord, glm::vec2 size, glm::vec3 colour, std::string shapeType = "rectangle", float radius = 0.0f);

    //checks if over or if pressed
    bool isMouseOver(glm::vec2 pos, glm::vec2 size);
    bool wasMousePressed(int button);

    struct anchors { //places on screen to keep items 'anchored' to
        glm::vec2 middleMiddle;
        glm::vec2 topMiddle;
        glm::vec2 bottomMiddle;

        glm::vec2 middleLeft;
        glm::vec2 topLeft;
        glm::vec2 bottomLeft;

        glm::vec2 middleRight;
        glm::vec2 topRight;
        glm::vec2 bottomRight;
    };
    anchors _anchors;

    double mouseCoord[2]; //keep track of mouse

private:
    GLuint fontTexture;
    GLuint VAO, VBO;
    // additional VAO/VBO for shapes (rectangles/circles) to avoid buffer collisions
    GLuint shapeVAO, shapeVBO;

    stbtt_bakedchar cdata[96]; //ASCII character set
    Shader* shader;
    glm::mat4 projection;
    float screenHeight;
    Shader* shapeShader;
    GLFWwindow* _window;
    bool mousePressed[3] = { false, false, false }; //left/middle/right
};

class button {
public:
    button(GUI* gui);
    ~button();
    void renderButton(const std::string& text, glm::vec2 coord, float fontScale, glm::vec3 fontColour, glm::vec2 btnSize, glm::vec3 colour, glm::vec2 txtOffset = glm::vec2(0, 0));
    void setOnClick(std::function<void()> callback); //sets a function to be called for click

    glm::vec2 btnCoords[2]; //btn coords
    GLFWwindow* _window;

private:
    GUI* gui; //pointer to GUI
    std::function<void()> onClick;
};


class slider {
public:
    slider(GUI* gui);
    ~slider();
    void renderSlider(const std::string& text, glm::vec2 coord, float fontScale, glm::vec3 fontColour, glm::vec2 size, glm::vec3 colour, glm::vec2 txtOffset, float btnRadius);

    glm::vec2 sliderBtnCoords[2]; //sliders btn coords
    GLFWwindow* _window;

private:
    float percentageAcross;
    GUI* gui; //pointer to GUI
};
