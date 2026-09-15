#ifndef MODEL_H
#define MODEL_H

#include <GL\GL.h>
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Mesh.h"
#include "Header.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

class Model
{
public:

    vector<Mesh> meshes;
    string directory;

    Model(string const& path) 
    {
        loadModel(path);
    }

    void Draw(SimpleShader shader, glm::mat4 transform_OX1, glm::mat4 transform_OX2, glm::mat4 transform_OX3)
    {
        for (int i = 0; i < meshes.size(); i++) {
            switch (i)
            {
            case 1:
               glm::vec3 minus_line_transform_OX1(-1.185, 0.6514, 0.05015);
               glm::vec3 line_transform_OX1(1.185, -0.6514, -0.05015);

               glm::mat4 line_OX1 = glm::translate(glm::mat4(1.0), line_transform_OX1);
               glm::mat4 minus_line_OX1 = glm::translate(glm::mat4(1.0), minus_line_transform_OX1);

               glm::mat4 Result_OX1 = line_OX1 * transform_OX1 * minus_line_OX1;
               shader.setUniform("transform", Result_OX1);

               break;

            case 2:
                glm::vec3 minus_line_transform_OX2(-0.8838, 0.542, 0.4588);
                glm::vec3 line_transform_OX2(0.8838, -0.542, -0.4588);

                glm::mat4 line_OX2 = glm::translate(glm::mat4(1.0), line_transform_OX2);
                glm::mat4 minus_line_OX2 = glm::translate(glm::mat4(1.0), minus_line_transform_OX2);

                glm::mat4 localTransform2 =line_OX2 * transform_OX2 * minus_line_OX2;
                glm::mat4 Result_OX2 = Result_OX1 * localTransform2;
                shader.setUniform("transform", Result_OX2);
               
                break;

            case 3:
                glm::vec3 minus_line_transform_OX3(-1.015, -0.8556, -0.03515);
                glm::vec3 line_transform_OX3(1.015, 0.8556, 0.03515);

                glm::mat4 line_OX3 = glm::translate(glm::mat4(1.0), line_transform_OX3);
                glm::mat4 minus_line_OX3 = glm::translate(glm::mat4(1.0), minus_line_transform_OX3);

                glm::mat4 localTransform3 = line_OX3 * transform_OX3 * minus_line_OX3;
                glm::mat4 Result_OX3 = Result_OX2 * localTransform3;
                shader.setUniform("transform", Result_OX3);

                break;
            default:
                shader.setUniform("transform", glm::mat4(1.0));
                break;
            }

            meshes[i].Draw();
        }
    }

private:

    void loadModel(string const& path)
    {
        Assimp::Importer importer;

        directory = path.substr(0, path.find_last_of('/'));

        const aiScene* scene = importer.ReadFile(directory,
            aiProcess_CalcTangentSpace |
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType);

        if (nullptr == scene) {
            fprintf(stderr, "ERROR ASSIMP:: %s\n", importer.GetErrorString());
        }


        processNode(scene->mRootNode, scene);

    }
   
    void processNode(aiNode* node, const aiScene* scene)
    {
        for (int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        for (int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        vector <Vertex>       vertices;
        vector <unsigned int> indices;


        for (int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vert_buff;
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vert_buff.Position = vector;

            if (mesh->HasNormals()) {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vert_buff.Normal = vector;
            }
            vertices.push_back(vert_buff);
        }


        for (int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        return Mesh(vertices, indices);
    }
};

#endif
