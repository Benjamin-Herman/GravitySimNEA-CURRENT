#include "../headers/shader.h"
#include <glad/glad.h>  // include before GLFW or else bad things happen
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    // get shader source code as string
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile; //take in vertex shader - verticies
    std::ifstream fShaderFile; //take in fragment shader - colour

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit); //fail if bad code
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try { //try catch cuz for some reason its more efficient
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close(); //all this stuff opens file, reads them, and closes

        vertexCode = vShaderStream.str(); //convert to strins
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl; // uh oh
    }

    const char* vShaderCode = vertexCode.c_str(); //convert to characters
    const char* fShaderCode = fragmentCode.c_str();

    // compile time
    unsigned int vertex, fragment;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER); //call openGL functions. it does it for me
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, "VERTEX");

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER); //same as vertex
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, "FRAGMENT");

    // shader program
    ID = glCreateProgram(); //still dont know what a shader program is. its just a block of shader stuff for the GPU
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment); //attach both colour data and vertex data
    glLinkProgram(ID); //link it to openGL
    CheckCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertex); //delete to save space and not get memory leaks because this is a useless language
    glDeleteShader(fragment);
}

void Shader::Use() {
    glUseProgram(ID); //call openGL stuff
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]); //more openGL stuff
}

void Shader::CheckCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024]; //reads through, checks for uncompiles signs and what not. tells me if there is
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << std::endl;
        }
    }
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); //more openGL
}

