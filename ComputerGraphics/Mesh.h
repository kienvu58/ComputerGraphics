#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <assimp\types.h>
#include <string>
#include <vector>

#include "Shader.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};


struct Texture
{
    GLuint id;
    std::string type;
    aiString path;
};


class Mesh
{
public:
    Mesh();
    Mesh(const std::vector<Vertex> & vertices,
        const std::vector<GLuint> & indices,
        const std::vector<Texture> & textures
    );
    ~Mesh();

    void Init(const std::vector<Vertex> & vertices,
        const std::vector<GLuint> & indices,
        const std::vector<Texture> & textures
    );

    void Render(Shader shader);
    void CleanUp();

private:
    void SetupMesh();

    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    std::vector<Texture> m_textures;

    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
};

