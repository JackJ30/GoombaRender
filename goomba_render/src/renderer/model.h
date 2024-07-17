#ifndef GOOMBARENDER_MODEL_H
#define GOOMBARENDER_MODEL_H

#include "renderer/vertex_array.h"
#include "renderer/asset.h"

#include <tiny_gltf.h>
#include <glm/glm.hpp>

namespace GoombaRender
{
    struct Mesh
    {
        VertexArray vao;
        std::vector<Texture2DAsset> textures; // replace with material
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
    
} // GoombaRender

#endif //GOOMBARENDER_MODEL_H
