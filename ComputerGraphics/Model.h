#pragma once
#include "Mesh.h"
#include <assimp\scene.h>

class Model
{
public:
    Model();
    Model(const GLchar * path);
    ~Model();

    void Init(const std::string & path);
    void Render(Shader shader);
    void CleanUp();

private:
    void ProcessNode(const aiNode * node, const aiScene * scene);
    Mesh ProcessMesh(const aiMesh * mesh, const aiScene * scene);
    std::vector<Texture> LoadMaterialTextures(const aiMaterial * material, 
        aiTextureType type, 
        const std::string& typeName
    );
    GLuint LoadTextureFromFile(const GLchar * path, std::string directory);

    std::string m_directory;
    std::vector<Mesh> m_meshes;
    std::vector<Texture> m_loadedTextures;
};

