#include "model.h"

namespace GoombaRender
{
    void Model::Create()
    {
        RequireContext();
        m_Created = true;
    }
    
    void Model::Delete()
    {
    
    }
    
    void Model::AddMesh(VertexArray vao, std::vector<Texture2DAsset> textures)
    {
        GLogInfo
        m_Meshes.push_back({vao, textures}); // TODO - make these moved
    }
} // GoombaRender