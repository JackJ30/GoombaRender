#include "material.h"

namespace GoombaRender
{
    Texture2D defaultTexture;
    bool defaultTextureInitialized = false;
    
    void CreateDefaultTexture(GoombaEngine::GraphicsContext& context)
    {
        defaultTexture.AssignContext(context);
        float white[] = {1.0, 1.0, 1.0, 1.0};
        defaultTexture.Create((unsigned char*)white, 1, 1, GL_RGBA, GL_FLOAT);
    }
    
    void Material::Create(Asset<ShaderInfo> shader)
    {
        RequireContext();
        
        m_Shader = shader;
        
        for (const auto& uniform : shader.Get().GetUniforms())
        {
            if (uniform.first == GL_SAMPLER_2D)
            {
                m_UnassignedTextures.insert(uniform.second);
            }
        }
        
        if (!defaultTextureInitialized) { CreateDefaultTexture(m_Context); }
        
        m_Created = true;
    }
    
    void Material::AssignTextureUniform(const std::string& name, const Asset<Texture2D>& texture)
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Model must be created assigning texture uniform.");
        
        if (m_UnassignedTextures.find(name) != m_UnassignedTextures.end())
        {
            m_Textures[name] = texture;
            m_UnassignedTextures.erase(name);
        }
    }
    
    void Material::Bind()
    {
        m_Shader.Get().Bind();
        m_UniformSettings.SetUniforms();
        
        int i = 0;
        for (const auto& texture : m_Textures)
        {
            m_Shader.Get().SetUniformInt(texture.first, i);
            texture.second.Get().Bind(i);
            
            ++i;
        }
        for (const std::string& unassigned : m_UnassignedTextures)
        {
            m_Shader.Get().SetUniformInt(unassigned, i);
            defaultTexture.Bind(i);
        }
    }
} // GoombaRender