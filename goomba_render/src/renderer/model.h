#ifndef GOOMBARENDER_MODEL_H
#define GOOMBARENDER_MODEL_H

#include "renderer/asset.h"
#include "renderer/texture.h"
#include "renderer/vertex_array.h"

#include <tiny_gltf.h>
#include <glm/glm.hpp>

namespace GoombaRender
{
    struct Mesh
    {
        VertexArray vao;
        std::vector<Asset<Texture2D>> textures; // replace with material
        glm::mat4 localTransform = glm::mat4(1.0);
    };
    
    class Model : public OglObj
    {
    public:
        void Create();
        void Delete();
        
        void AddMesh(const Mesh& mesh);
        
        // TODO - these should really be const
        inline std::vector<unsigned int>& GetBuffers() { return m_Buffers; }
        const inline std::vector<Mesh>& GetMeshes() { return m_Meshes; }
        
        
    private:
        std::vector<unsigned int> m_Buffers;
        std::vector<Mesh> m_Meshes;
    };
    
    void LoadModel(Asset<Model>& asset, GoombaEngine::GraphicsContext& context);
    
} // GoombaRender

#endif //GOOMBARENDER_MODEL_H
