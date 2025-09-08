#include "../headers/GUI.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "../headers/stb_truetype.h"

#include <fstream>
#include <vector>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <algorithm>

bool overBtn = false;

GUI::GUI(GLFWwindow* win) {
    //VAO and VBO init
    _window = win;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    // create separate VAO/VBO for shapes (rects/circles) so we can upload variable-size fans safely
    glGenVertexArrays(1, &shapeVAO);
    glGenBuffers(1, &shapeVBO);
    glBindVertexArray(shapeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, shapeVBO);
    // bigger buffer for circles
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 1024 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    //load text shader
    shader = new Shader("shaders/textShader.vert", "shaders/textShader.frag");

    //load shape shader
    shapeShader = new Shader("shaders/shapeShader.vert", "shaders/shapeShader.frag");

    //projection to match standard window size
    projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f);
    shader->Use();
    shader->SetMat4("projection", projection);
    shapeShader->Use();
    shapeShader->SetMat4("projection", projection);
    //glfwSetMouseButtonCallback(_window, mouse_button_callback);
}


GUI::~GUI() {
    glDeleteVertexArrays(1, &VAO); //delete all vertex and buffers
    glDeleteBuffers(1, &VBO);
    // delete shape VAO/VBO
    glDeleteVertexArrays(1, &shapeVAO);
    glDeleteBuffers(1, &shapeVBO);

    glDeleteTextures(1, &fontTexture);
    if (shader) {
        delete shader; //if called and this exists, delete cuz its the delety thingy
    }
    if (shapeShader) {
        delete shapeShader; //if called and this exists, delete cuz its the delety thingy
    }
}

bool GUI::loadFont(const char* ttf_path, float pixel_height) {
    std::ifstream file(ttf_path, std::ios::binary); //take code in
    if (!file.is_open()) {
        return false;
    }

    file.seekg(0, std::ios::end); //goes to end to see the end pretty dumb
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    //makes buffers for them all
    std::vector<unsigned char> ttf_buffer(size);
    file.read((char*)ttf_buffer.data(), size);

    //making the image in code
    const int bitmapWidth = 512;
    const int bitmapHeight = 512;
    std::vector<unsigned char> bitmap(bitmapWidth * bitmapHeight);

    //some library stuff
    if (stbtt_BakeFontBitmap(ttf_buffer.data(), 0, pixel_height, bitmap.data(), bitmapWidth, bitmapHeight, 32, 96, cdata) <= 0) {
        std::cout << "Failed to bake font\n";
        return false;
    }
    //magic texture creation tools. again library documentation here. i get it but i dont get the code
    glGenTextures(1, &fontTexture);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmapWidth, bitmapHeight, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

void GUI::renderText(const std::string& text, glm::vec2 coord, float scale, glm::vec3 colour) {
    float r = colour.x;
    float g = colour.y;
    float b = colour.z; //splits vec3 into 3 floats

    float x = coord.x;
    float y = coord.y;

    shader->Use();
    shader->SetVec3("textColor", glm::vec3(r, g, b));
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, fontTexture);

    float xpos, ypos, w, h;
    float vertices[6][4];

    for (char c : text) {
        if (c < 32 || c >= 128) { //not in the right character set
            continue;
        }

        stbtt_bakedchar* b = cdata + (c - 32); //shifting to get right data

        xpos = x + b->xoff * scale;
        ypos = y + (b->yoff * scale); //invert for top left origin because thats how this works
        w = b->x1 - b->x0;
        h = b->y1 - b->y0;

        //6 verticies per quad
        vertices[0][0] = xpos;     vertices[0][1] = ypos;     vertices[0][2] = b->x0 / 512.0f; vertices[0][3] = b->y0 / 512.0f;
        vertices[1][0] = xpos + w; vertices[1][1] = ypos;     vertices[1][2] = b->x1 / 512.0f; vertices[1][3] = b->y0 / 512.0f;
        vertices[2][0] = xpos;     vertices[2][1] = ypos + h; vertices[2][2] = b->x0 / 512.0f; vertices[2][3] = b->y1 / 512.0f;

        vertices[3][0] = xpos + w; vertices[3][1] = ypos;     vertices[3][2] = b->x1 / 512.0f; vertices[3][3] = b->y0 / 512.0f;
        vertices[4][0] = xpos + w; vertices[4][1] = ypos + h; vertices[4][2] = b->x1 / 512.0f; vertices[4][3] = b->y1 / 512.0f;
        vertices[5][0] = xpos;     vertices[5][1] = ypos + h; vertices[5][2] = b->x0 / 512.0f; vertices[5][3] = b->y1 / 512.0f;

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        //makes it so that its the right size
        x += b->xadvance * scale;
    }
}

void GUI::updateSize(GLFWwindow* window) {
    int height, width; //creates variables for function call next code

    glfwGetWindowSize(window, &width, &height);
    screenHeight = height;
    projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f); //origin
    shader->Use(); //use the stuff
    shader->SetMat4("projection", projection);
    float fHeight = static_cast<float>(height);
    float fWidth = static_cast<float>(width);
    //plus anchor stuff

    //bottom
    _anchors.bottomLeft = glm::vec2{ 0, fHeight };
    _anchors.bottomMiddle = glm::vec2{ fWidth / 2, fHeight };
    _anchors.bottomRight = glm::vec2{ fWidth, fHeight };

    //middle
    _anchors.middleLeft = glm::vec2{ 0, fHeight / 2 };
    _anchors.middleMiddle = glm::vec2{ fWidth / 2, fHeight / 2 };
    _anchors.middleRight = glm::vec2{ fWidth, fHeight / 2 };

    //top
    _anchors.topLeft = glm::vec2{ 0, 0 };
    _anchors.topMiddle = glm::vec2{ fWidth / 2, 0 };
    _anchors.topRight = glm::vec2{ fWidth, 0 };

    glfwGetCursorPos(window, &mouseCoord[0], &mouseCoord[1]);
}

void GUI::renderShape(glm::vec2 coord, glm::vec2 size, glm::vec3 colour, std::string shapeType, float radius) {
    if (shapeType == "rectangle") {
        shapeShader->Use();
        shapeShader->SetMat4("projection", projection);
        shapeShader->SetVec3("shapeColor", colour);

        float x = coord.x;
        float y = coord.y; //coordinates already in top-left origin
        float w = size.x;
        float h = size.y;

        //6 vertices for two triangles, 4 floats per vertex (x,y,u,v) to match VAO layout
        float vertices[6][4] = {
            { x,     y,     0.0f, 0.0f },
            { x + w, y,     0.0f, 0.0f },
            { x + w, y + h, 0.0f, 0.0f },

            { x,     y,     0.0f, 0.0f },
            { x + w, y + h, 0.0f, 0.0f },
            { x,     y + h, 0.0f, 0.0f }
        };

        glBindVertexArray(shapeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, shapeVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    else if (shapeType == "circle") {
        shapeShader->Use();
        shapeShader->SetMat4("projection", projection);
        shapeShader->SetVec3("shapeColor", colour);

        // Circle center and radius
        float cx = coord.x + size.x * 0.5f;
        float cy = coord.y + size.y * 0.5f;
        const float PI = 3.14159265359f; // local definition for circle

        // fallback radius if caller passed 0
        float r = (radius > 0.0f) ? radius : std::min(size.x, size.y) * 0.5f;

        const int segments = 64; // number of triangles for smoothness
        std::vector<float> vertices;
        vertices.reserve((segments + 2) * 4);

        // center vertex
        vertices.push_back(cx);
        vertices.push_back(cy);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);

        for (int i = 0; i <= segments; i++) {
            float angle = i * 2.0f * PI / segments;
            float px = cx + std::cos(angle) * r;
            float py = cy + std::sin(angle) * r;

            vertices.push_back(px);
            vertices.push_back(py);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }

        glBindVertexArray(shapeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, shapeVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());

        glDrawArrays(GL_TRIANGLE_FAN, 0, segments + 2);
    }


    // Optional: template for other shapes
    /*
    if(shapeType == "triangle") {
        // compute triangle vertices in top-left ortho
    }
    else if(shapeType == "circle") {
        // generate circle vertices in top-left ortho
    }
    */
}

bool GUI::isMouseOver(glm::vec2 pos, glm::vec2 size) {
    return mouseCoord[0] > pos.x &&
        mouseCoord[0] < pos.x + size.x &&
        mouseCoord[1] > pos.y &&
        mouseCoord[1] < pos.y + size.y;
}

bool GUI::wasMousePressed(int button) {
    return glfwGetMouseButton(_window, button) == GLFW_PRESS;
}

button::button(GUI* g) : gui(g) {}

button::~button() {
}

void button::setOnClick(std::function<void()> callback) {
    onClick = callback;
}

void button::renderButton(const std::string& text, glm::vec2 coord, float fontScale, glm::vec3 fontColour, glm::vec2 btnSize, glm::vec3 colour, glm::vec2 txtOffset) {
    //render shape
    //glm::vec2 rectTopLeft = coord;
    glm::vec2 rectTopRight = coord + glm::vec2(btnSize.x, 0);
    glm::vec2 rectBottomLeft = coord + glm::vec2(0, btnSize.y);
    //glm::vec2 rectBottomRight = coord + btnSize;

    //stores as variable
    //btnCoords[0] = rectTopLeft;
    btnCoords[0] = rectTopRight;
    btnCoords[1] = rectBottomLeft;
    //btnCoords[3] = rectBottomRight;
    //std::cout << rectTopLeft[0] << " " << rectTopLeft[1] << "\n";
    //std::cout << rectTopRight[0] << " " << rectTopRight[1] << "\n";
    //std::cout << rectBottomLeft[0] << " " << rectBottomLeft[1] << "\n";
    //std::cout << rectBottomRight[0] << " " << rectBottomRight[1] << "\n";

    if (gui->mouseCoord[0] > rectBottomLeft[0] && gui->mouseCoord[0] < rectTopRight[0] &&
        gui->mouseCoord[1] < rectBottomLeft[1] && gui->mouseCoord[1] > rectTopRight[1]) {
        colour *= 0.7;
        overBtn = true;

        static bool prevMouseState = false;
        bool currentMouseState = gui->wasMousePressed(GLFW_MOUSE_BUTTON_LEFT);

        //check for up and down
        if (currentMouseState && !prevMouseState && onClick) {
            onClick(); // call callback
        }

        prevMouseState = currentMouseState;
    }
    else {
        overBtn = false;
    }
    gui->renderShape(coord, btnSize, colour, "rectangle");
    //std::cout << mouseCoord[0] << " " << mouseCoord[1] << "\n";
    //260 150
    //610 150
    //260 225
    //610 225

    coord += txtOffset;
    //render the text on top of the button
    gui->renderText(text, coord, fontScale, fontColour);
}

slider::slider(GUI* g) : gui(g) {}

slider::~slider() {}

void slider::renderSlider(const std::string& text, glm::vec2 coord, float fontScale, glm::vec3 fontColour, glm::vec2 size, glm::vec3 colour, glm::vec2 txtOffset, float btnRadius) {
    //render shape
    //glm::vec2 rectTopLeft = coord;
    glm::vec2 rectTopRight = coord + glm::vec2(size.x, 0);
    glm::vec2 rectBottomLeft = coord + glm::vec2(0, size.y);
    //glm::vec2 rectBottomRight = coord + btnSize;

    //stores as variable
    //btnCoords[0] = rectTopLeft;
    sliderBtnCoords[0] = rectTopRight;
    sliderBtnCoords[1] = rectBottomLeft;
    //btnCoords[3] = rectBottomRight;
    //std::cout << rectTopLeft[0] << " " << rectTopLeft[1] << "\n";
    //std::cout << rectTopRight[0] << " " << rectTopRight[1] << "\n";
    //std::cout << rectBottomLeft[0] << " " << rectBottomLeft[1] << "\n";
    //std::cout << rectBottomRight[0] << " " << rectBottomRight[1] << "\n";

    gui->renderShape(coord, size, colour, "rectangle");

    // Circle center
    static float buttonX = coord.x; // store the button horizontal position
    float cy = coord.y + size.y * 0.5f; // vertical center of rectangle
    float r = (btnRadius > 0.0f) ? btnRadius : size.y * 0.5f; // fallback radius if not specified

    // check if mouse is inside the circle
    float dx = gui->mouseCoord[0] - (buttonX + r);
    float dy = gui->mouseCoord[1] - cy;
    if ((dx * dx + dy * dy) <= r * r) { // distance squared < radius squared
        colour *= 0.7;
        overBtn = true;

        static bool prevMouseState = false;
        bool currentMouseState = gui->wasMousePressed(GLFW_MOUSE_BUTTON_LEFT);

        if (currentMouseState) {
            // move button along the slider track but clamp it
            buttonX = gui->mouseCoord[0] - r;
            if (buttonX < coord.x) buttonX = coord.x;
            if (buttonX > coord.x + size.x - 2 * r) buttonX = coord.x + size.x - 2 * r;
        }
        
        prevMouseState = currentMouseState;


        //percentage calc
        percentageAcross = (buttonX - coord.x) / (size.x - 2 * r) * 100;
        //std::cout << "PERCENTAGE " << percentageAcross << "\n";
    }
    else {
        overBtn = false;
    }

    // render the circle, horizontally movable but vertically centered
    gui->renderShape(glm::vec2{ buttonX, cy - r }, glm::vec2{ 2 * r, 2 * r }, colour, "circle", r);
}
