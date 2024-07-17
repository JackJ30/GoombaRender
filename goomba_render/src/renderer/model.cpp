#include "model.h"

#include <utility>

namespace GoombaRender
{
    void Model::Create()
    {
        RequireContext();
        m_Created = true;
    }
    
    void Model::Delete()
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Model must be created before deleting.");
        
        for (auto& mesh : m_Meshes)
        {
            mesh.vao.Delete();
        }
        m_Context.GetGlad().DeleteBuffers(m_Buffers.size(), m_Buffers.data());
    }
    
    void Model::AddMesh(const Mesh& mesh)
    {
        m_Meshes.push_back(mesh); // TODO - move this
    }
} // GoombaRender