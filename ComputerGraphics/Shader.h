#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <string>


class Shader
{
public:
    Shader();
    ~Shader();

    void Init(const GLchar * vertexPath, const GLchar * fragmentPath);
    void UseProgram() const;

    void SetUniform(const char * name, const glm::mat4 & mat);
    void SetUniform(const char * name, int val);
private:

    std::string ReadFile(const GLchar * filename);
    void CheckErrors(GLuint shader, std::string type);

    GLuint m_program;
};

