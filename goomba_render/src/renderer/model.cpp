#include "model.h"

#include <glm/gtc/quaternion.hpp>
#include <utility>

namespace GoombaRender
{
    Model::Model(std::vector<Mesh> meshes, std::vector<unsigned int> buffers)
        : m_Meshes(std::move(meshes)), m_Buffers(std::move(buffers))
    {
    
    }
    
    void Model::Delete()
    {
        for (auto& mesh : m_Meshes)
        {
            mesh.vao->Delete();
        }
        glad.DeleteBuffers(m_Buffers.size(), m_Buffers.data());
    }
} // GoombaRender