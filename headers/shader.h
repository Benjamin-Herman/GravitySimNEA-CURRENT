#pragma once
#include <glm/glm.hpp>
#include <string>

class Shader {
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    void Use();
    void SetMat4(const std::string& name, const glm::mat4& mat) const;

private:
    unsigned int ID;
    void CheckCompileErrors(unsigned int shader, std::string type);
};