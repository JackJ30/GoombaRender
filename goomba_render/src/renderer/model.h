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
        Mesh() = default;
        Mesh(std::shared_ptr<VertexArrayInfo> vao, std::shared_ptr<Material> material)
            : vao(std::move(vao)), material(std::move(material))
        {
        
        }
        
        std::shared_ptr<VertexArrayInfo> vao = nullptr;
        std::shared_ptr<Material> material = nullptr;
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
