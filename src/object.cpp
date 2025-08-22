#include "../headers/Object.h"
#include "../headers/time.h"

#include <glad/glad.h>
#include <iostream>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
Object::Object(const std::string& objFilePath) : position(0.0f, 0.0f, 0.0f), rotationAngle(0.0f) {
    //try to load from OBJ file and if not fall back to cube
    if (!LoadFromFile(objFilePath)) {
        // fallback to default cube if OBJ loading fails
        // Object vertices with colors (8 vertices, each with 3 position + 3 color)
        // Object verts with colours. 8 verts. 3 pos and 3 colour
        vertices = {
            // positions          // colours
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f
        }; // a list of all verticies

        // Object indices list. if not like this then memory is wasted :(
        indices = {
            0, 1, 2, 2, 3, 0, // front
            1, 5, 6, 6, 2, 1, // right
            5, 4, 7, 7, 6, 5, // back
            4, 0, 3, 3, 7, 4, // left
            3, 2, 6, 6, 7, 3, // top
            4, 5, 1, 1, 0, 4  // bottom
        };
    }

    SetupMesh();
}

bool Object::LoadFromFile(const std::string& filePath) {
    //clear existing data
    vertices.clear();
    indices.clear();

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    //load the OBJ file using tinyobjloader
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath.c_str())) {
        std::cout << "Failed to load OBJ: " << warn + err << std::endl;
        return false;
    }
    float maxDistSq = 0.0f;

    //process each shape in the OBJ file
    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            //get vertex position from OBJ
            float x = attrib.vertices[3 * index.vertex_index + 0];
            float y = attrib.vertices[3 * index.vertex_index + 1];
            float z = attrib.vertices[3 * index.vertex_index + 2];

            float distSq = x * x + y * y + z * z;
            if (distSq > maxDistSq) {
                maxDistSq = distSq;
            }


            //add position to vertices
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            //add color
            //vertices.push_back(0.5f); //red
            //vertices.push_back(0.5f); //green
            //vertices.push_back(0.5f); //blue
            vertices.push_back((x + 0.5f) * 0.8f); //red 
            vertices.push_back((y + 0.5f) * 0.8f); //green 
            vertices.push_back((z + 0.5f) * 0.8f); //blue 

            //add indexes
            indices.push_back(indices.size());
        }
    }
    radius = std::sqrt(maxDistSq);
    //std::cout << radius;
    return true;
}



void Object::Update(float dt) {
    //float dt = Time::DeltaTime();
    bool rot = false;
    if (rot) {
        rotationAngle += 50.0f * dt; //tell angle to go brrrr - srs tho it turns the Object by a certain amount by dt
        if (rotationAngle > 360.0f) {
            rotationAngle -= 360.0f;
        }
    }
    //std::cout << "SPHERE: " << acceleration.x << " " << acceleration.y << " " << acceleration.z << "\n";
   
    position += velocity * dt * simSpeed; 
    velocity += acceleration * dt * simSpeed;


}

glm::mat4 Object::GetModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f); //init matrix 4 dimentional. makes rotation and movement calc easy
    model = glm::translate(model, position); // calls function to change matrix
    model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.5f, 1.0f, 0.0f)); //same stuff but rotate
    return model;
}

void Object::setVelocity(glm::vec3 vel){
    velocity = vel;
}

void Object::setAcceleration(glm::vec3 accel){
    acceleration = accel;
}

void Object::setPosition(glm::vec3 pos){
    position = pos;
}

void Object::SetupMesh() {
    glGenVertexArrays(1, &VAO); //vertex array and buffer objects, and element buffer object
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    //this is all memeory management and storing data for the grpahics card

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // pos attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // colour attribute. maths stuff for the graphics card not to scream
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Object::Render() {
    glBindVertexArray(VAO); // the vertex array object goes to openGL and it tells graphics card what to do 
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); //gives the indicies so it know whats where
    glBindVertexArray(0); //i dont know what this is but it crashes without it
}

std::string Object::getType() {
    return type;
}