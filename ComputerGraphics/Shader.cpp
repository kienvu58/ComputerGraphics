#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm\gtc\type_ptr.hpp>

Shader::Shader() : m_program(0)
{
}

Shader::~Shader()
{
    glDeleteProgram(m_program);
}

void Shader::Init(const GLchar * vertexPath, const GLchar * fragmentPath)
{
    std::string vertexString = ReadFile(vertexPath);
    const GLchar * vertexCode = vertexString.c_str();

    std::string fragmentString = ReadFile(fragmentPath);
    const GLchar * fragmentCode = fragmentString.c_str();

    // compile vertex shader
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, nullptr);
    glCompileShader(vertex);
    CheckErrors(vertex, "VERTEX");

    // compile fragment shader
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, nullptr);
    glCompileShader(fragment);
    CheckErrors(fragment, "FRAGMENT");

    // link program
    m_program = glCreateProgram();
    glAttachShader(m_program, vertex);
    glAttachShader(m_program, fragment);
    glLinkProgram(m_program);
    CheckErrors(m_program, "PROGRAM");

    // clean up
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::UseProgram() const
{
    glUseProgram(m_program);
}

void Shader::SetUniform(const char * name, const glm::mat4 & mat)
{
    GLuint location = glGetUniformLocation(m_program, name);
    if (location != -1)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    }
}

void Shader::SetUniform(const char * name, int val)
{
    GLuint location = glGetUniformLocation(m_program, name);
    if (location != -1)
    {
        glUniform1i(location, val);
    }
}

std::string Shader::ReadFile(const GLchar * filename)
{
    std::string content;
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        file.open(filename);
        std::stringstream stream;
        stream << file.rdbuf();
        file.close();
        content = stream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cerr << "[ERROR] reading file " << filename << std::endl;
    }
    return content;
}

void Shader::CheckErrors(GLuint shader, std::string type)
{
    GLint success;
    GLchar infoLog[GL_INFO_LOG_LENGTH];
    if (type == "PROGRAM")
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, GL_INFO_LOG_LENGTH, nullptr, infoLog);
            std::cerr << "[ERROR] linking " << type << "\n" << infoLog << std::endl;
        }
    }
    else
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, GL_INFO_LOG_LENGTH, nullptr, infoLog);
            std::cerr << "[ERROR] compiling " << type << "\n" << infoLog << std::endl;
        }
    }
}
