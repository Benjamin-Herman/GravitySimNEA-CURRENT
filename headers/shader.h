#pragma once
#ifndef SHADER_H
#define SHADER_H


#include <glm/glm.hpp>
#include <string>

class Shader {
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    void Use();
    void SetMat4(const std::string& name, const glm::mat4& mat) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;

private:
    unsigned int ID; //shader id so I know what one is what
    void CheckCompileErrors(unsigned int shader, std::string type);
};

#endif
