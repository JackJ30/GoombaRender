#include "material.h"

namespace GoombaRender
{
    void Material::Create(Asset<Shader> shader)
    {
        RequireContext();
        
        m_Shader = shader;
        m_Created = true;
    }
    
    void Material::AssignTextureUniform(std::string name, Asset<Texture2D> texture)
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Model must be created assigning texture uniform.");
        
        m_UniformSettings.ints.emplace_back(name, m_Textures.size());
        m_Textures.push_back(texture);
    }
    
    void Material::Bind() const
    {
        m_Shader.Get().Bind();
        m_Shader.Get().SetUniforms(m_UniformSettings);
        
        for (int i = 0; i < m_Textures.size(); ++i)
        {
            m_Textures[i].Get().Bind(i);
        }
    }
} // GoombaRender