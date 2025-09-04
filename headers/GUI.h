#ifndef GUI_H
#define GUI_H

#include <glad/glad.h>   // Must come before GLFW
#include <glm/glm.hpp>
#include <string>
#include "shader.h"
#include "stb_truetype.h" // Just the header, no implementation macro here

class GUI {
public:
    GUI(int width, int height);
    ~GUI();

    void loadFont(const char* fontPath);
    void renderText(const std::string& text, float x, float y, float scale, float r, float g, float b);

    void bindFramebuffer();
    void unbindFramebuffer();

    GLuint getTexture() const { return texture; }
    GLuint getQuadVAO() const { return quadVAO; }

private:
    int windowWidth, windowHeight;

    GLuint fbo = 0;
    GLuint texture = 0;
    GLuint rbo = 0;

    GLuint quadVAO = 0;
    GLuint quadVBO = 0;

    GLuint fontTexture = 0;
    stbtt_bakedchar cdata[96];

    Shader* guiShader = nullptr;

    void initFramebuffer();
    void initQuad();
    void initShader();
};

#endif // GUI_H
