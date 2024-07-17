#ifndef GOOMBARENDER_MODEL_H
#define GOOMBARENDER_MODEL_H

#include "renderer/vertex_array.h"
#include "renderer/asset.h"

#include <tiny_gltf.h>

namespace GoombaRender
{
    struct Mesh
    {
        VertexArray vao;
        std::vector<Texture2DAsset> textures;
    };
    
    class Model : public OglObj
    {
    public:
        void Create();
        void Delete();
        
        void AddMesh(const Mesh& mesh);
        inline std::vector<unsigned int>& GetBuffers() { return m_Buffers; }
        
        inline size_t GetMeshCount() const { return m_Meshes.size(); }
        
    private:
        std::vector<unsigned int> m_Buffers;
        std::vector<Mesh> m_Meshes;
    };
    
} // GoombaRender

#endif //GOOMBARENDER_MODEL_H
