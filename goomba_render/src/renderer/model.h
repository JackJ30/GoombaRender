#ifndef GOOMBARENDER_MODEL_H
#define GOOMBARENDER_MODEL_H

#include "renderer/asset.h"
#include "renderer/texture.h"
#include "renderer/vertex_array.h"
//#include "renderer/material.h"

#include <tiny_gltf.h>
#include <glm/glm.hpp>

namespace GoombaRender
{
    struct Mesh
    {
        VertexArray vao;
        //Asset<Material> material;
        glm::mat4 localTransform = glm::mat4(1.0);
    };
    
    class Model
    {
    public:
        void Create();
        void Delete();
        
        void AddMesh(const Mesh& mesh);
        
        inline std::vector<unsigned int>& GetBuffers() { return m_Buffers; } // TODO - this should really be const
        inline const std::vector<Mesh>& GetMeshes() const { return m_Meshes; }
        
        
    private:
        std::vector<unsigned int> m_Buffers;
        std::vector<Mesh> m_Meshes;
    };
    
    // TODO - support multiple uv maps
    void InitializeModelLoader(std::string pbrShaderPath);
    void LoadModel(Asset<Model>& asset, GoombaEngine::GraphicsContext& context);
    
} // GoombaRender

#endif //GOOMBARENDER_MODEL_H
