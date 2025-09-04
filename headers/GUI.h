#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "stb_truetype.h"
#include "shader.h"
#include <glm/glm.hpp>

class GUI {
public:
    GUI();
    ~GUI();

    bool loadFont(const char* ttf_path, float pixel_height);
    void renderText(const std::string& text, glm::vec2 coord, float scale, glm::vec3 colour);
    void updateSize(GLFWwindow* window);
    void renderShape(glm::vec2 coord, glm::vec2 size, glm::vec3 colour, std::string shapeType = "rectangle");
    void renderButton(const std::string& text, glm::vec2 coord, float fontScale, glm::vec3 fontColour, glm::vec2 btnSize, glm::vec3 colour, glm::vec2 txtOffset, void* command);
    struct anchors {
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


private:
    GLuint fontTexture;
    GLuint VAO, VBO;
    stbtt_bakedchar cdata[96]; //ascci character set

    Shader* shader;
    glm::mat4 projection;
    float screenHeight;
    Shader* shapeShader;
};
