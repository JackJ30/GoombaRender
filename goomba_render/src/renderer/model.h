#ifndef GOOMBARENDER_MODEL_H
#define GOOMBARENDER_MODEL_H

#include "renderer/texture.h"
#include "renderer/vertex_array.h"
#include "renderer/material.h"

#include <glm/glm.hpp>

namespace GoombaRender
{
    struct Mesh
    {
        VertexArrayInfo vao;
        std::shared_ptr<Material> material;
        glm::mat4 localTransform = glm::mat4(1.0);
    };
    
    class Model
    {
    public:
        Model(std::vector<Mesh> meshes, std::vector<unsigned int> buffers);
        void Delete();
        
        inline const std::vector<Mesh>& GetMeshes() const { return m_Meshes; }
    private:
        std::vector<Mesh> m_Meshes;
        std::vector<unsigned int> m_Buffers;
    };
    
} // GoombaRender

#endif //GOOMBARENDER_MODEL_H
