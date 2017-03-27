#include "Model.h"
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <FreeImage.h>
#include <iostream>



Model::Model()
{
}

Model::Model(const GLchar * path)
{
    Init(path);
}


Model::~Model()
{
}

void Model::Init(const std::string & path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

    // check for errors
    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "[ERROR] assimp importing: " << importer.GetErrorString() << std::endl;
        return;
    }

    m_directory = path.substr(0, path.find_last_of('/'));
    ProcessNode(scene->mRootNode, scene);
}

void Model::Render(Shader shader)
{
    for (Mesh& mesh : m_meshes)
    {
        mesh.Render(shader);
    }
}

void Model::CleanUp()
{
    for (Mesh & mesh : m_meshes)
    {
        mesh.CleanUp();
    }
}

void Model::ProcessNode(const aiNode * node, const aiScene * scene)
{
    for (GLuint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(ProcessMesh(mesh, scene));
    }

    for (GLuint i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(const aiMesh * mesh, const aiScene * scene)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    // vertices
    for (GLuint i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;
        // position
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
        // normal
        if (mesh->mNormals)
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
        }
        else
        {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }

    // faces
    for (GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (GLuint j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // materials 
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        // diffuse maps
        std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        // specular maps
        std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    Mesh newMesh(vertices, indices, textures);
    return newMesh;
}

std::vector<Texture> Model::LoadMaterialTextures(const aiMaterial * material, aiTextureType type, const std::string & typeName)
{
    std::vector<Texture> textures;
    for (GLuint i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString str;
        material->GetTexture(type, i, &str);

        // check if texture was loaded before 
        // and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (GLuint j = 0; j < m_loadedTextures.size(); j++)
        {
            if (std::strcmp(m_loadedTextures[j].path.C_Str(), str.C_Str()) == 0)
            {
                textures.push_back(m_loadedTextures[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            Texture texture;
            texture.id = LoadTextureFromFile(str.C_Str(), m_directory);
            texture.type = typeName;
            texture.path = str;
            textures.push_back(texture);
            m_loadedTextures.push_back(texture);
        }
    }
    return textures;
}

GLuint Model::LoadTextureFromFile(const GLchar * path, std::string directory)
{
    std::string filename = std::string(path);
    filename = directory + "/" + filename;

    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename.c_str());
    FIBITMAP *bitmap = FreeImage_Load(format, filename.c_str());
    int bitsPerPixel = FreeImage_GetBPP(bitmap);
    FIBITMAP *bitmap32;
    if (bitsPerPixel == 32)
    {
        bitmap32 = bitmap;
    }
    else
    {
        bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
    }

    int width = FreeImage_GetWidth(bitmap32);
    int height = FreeImage_GetHeight(bitmap32);

    GLubyte *image = FreeImage_GetBits(bitmap32);

    GLuint textureId;
    glGenTextures(1, &textureId);

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    FreeImage_Unload(bitmap32);
    if (bitsPerPixel != 32)
    {
        FreeImage_Unload(bitmap);
    }

    return textureId;
}
