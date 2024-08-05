#include "material.h"
#include <stb_image.h>

namespace GoombaRender
{
    std::unique_ptr<Texture2DInfo> defaultTexture;
    
    void CreateDefaultTexture()
    {
        float white[] = {1.0, 1.0, 1.0, 1.0};
        defaultTexture = std::make_unique<Texture2DInfo>(CreateTexture2D((unsigned char*)white, 1, 1, GL_RGBA, GL_FLOAT));
    }
    
    Material::Material(std::shared_ptr<ShaderInfo> shader)
        : m_Shader(shader)
    {
        for (const auto& uniform : shader->GetUniforms())
        {
            m_UniformNames.push_back(uniform.second);
            
            if (uniform.first == GL_SAMPLER_2D)
            {
                m_UnassignedTextures.insert(uniform.second);
            }
        }
        
        if (defaultTexture == nullptr) { CreateDefaultTexture(); }
    }
    
    void Material::Bind() const
    {
        m_Shader->Bind();
        
        int i = 0;
        for (const auto& texture : m_Textures)
        {
            m_Shader->SetUniformInt(texture.first, i);
            texture.second->Bind(i);
            
            ++i;
        }
        if (!m_UnassignedTextures.empty()) { defaultTexture->Bind(i); };
        for (const std::string& unassigned : m_UnassignedTextures)
        {
            m_Shader->SetUniformInt(unassigned, i);
        }
        
        m_UniformSettings.SetUniforms(*m_Shader);
    }
    
    void Material::AssignUniformTexture(const std::string& name, std::shared_ptr<Texture2DInfo> texture)
    {
        if (m_UnassignedTextures.find(name) != m_UnassignedTextures.end())
        {
            m_Textures[name] = texture;
            m_UnassignedTextures.erase(name);
        }
    }
    
    void Material::AssignUniformBool(const std::string &name, bool value)
    {
        if (std::find(m_UniformNames.begin(), m_UniformNames.end(), name) != m_UniformNames.end())
        {
            m_UniformSettings.bools.emplace_back(name, value);
        }
        else { GLogError("Uniform '{}' does not exist for this shader.", name); }
    }
    
    void Material::AssignUniformInt(const std::string &name, int value)
    {
        if (std::find(m_UniformNames.begin(), m_UniformNames.end(), name) != m_UniformNames.end())
        {
            m_UniformSettings.ints.emplace_back(name, value);
        }
        else { GLogError("Uniform '{}' does not exist for this shader.", name); }
    }
    
    void Material::AssignUniformFloat(const std::string &name, float value)
    {
        if (std::find(m_UniformNames.begin(), m_UniformNames.end(), name) != m_UniformNames.end())
        {
            m_UniformSettings.floats.emplace_back(name, value);
        }
        else { GLogError("Uniform '{}' does not exist for this shader.", name); }
    }
    
    void Material::AssignUniformVec2(const std::string &name, const glm::vec2 &value)
    {
        if (std::find(m_UniformNames.begin(), m_UniformNames.end(), name) != m_UniformNames.end())
        {
            m_UniformSettings.vec2s.emplace_back(name, value);
        }
        else { GLogError("Uniform '{}' does not exist for this shader.", name); }
    }
    
    void Material::AssignUniformVec2(const std::string &name, float x, float y)
    {
        if (std::find(m_UniformNames.begin(), m_UniformNames.end(), name) != m_UniformNames.end())
        {
            m_UniformSettings.vec2s.emplace_back(name, glm::vec2(x, y));
        }
        else { GLogError("Uniform '{}' does not exist for this shader.", name); }
    }
    
    void Material::AssignUniformVec3(const std::string &name, const glm::vec3 &value)
    {
        if (std::find(m_UniformNames.begin(), m_UniformNames.end(), name) != m_UniformNames.end())
        {
            m_UniformSettings.vec3s.emplace_back(name, value);
        }
        else { GLogError("Uniform '{}' does not exist for this shader.", name); }
    }
    
    void Material::AssignUniformVec3(const std::string &name, float x, float y, float z)
    {
        if (std::find(m_UniformNames.begin(), m_UniformNames.end(), name) != m_UniformNames.end())
        {
            m_UniformSettings.vec3s.emplace_back(name, glm::vec3(x, y, z));
        }
        else { GLogError("Uniform '{}' does not exist for this shader.", name); }
    }
    
    void Material::AssignUniformVec4(const std::string &name, const glm::vec4 &value)
    {
        if (std::find(m_UniformNames.begin(), m_UniformNames.end(), name) != m_UniformNames.end())
        {
            m_UniformSettings.vec4s.emplace_back(name, value);
        }
        else { GLogError("Uniform '{}' does not exist for this shader.", name); }
    }
    
    void Material::AssignUniformVec4(const std::string &name, float x, float y, float z, float w)
    {
        if (std::find(m_UniformNames.begin(), m_UniformNames.end(), name) != m_UniformNames.end())
        {
            m_UniformSettings.vec4s.emplace_back(name, glm::vec4(x, y, z, w));
        }
        else { GLogError("Uniform '{}' does not exist for this shader.", name); }
    }
    
    void Material::AssignUniformMat2(const std::string &name, const glm::mat2 &mat)
    {
        if (std::find(m_UniformNames.begin(), m_UniformNames.end(), name) != m_UniformNames.end())
        {
            m_UniformSettings.mat2s.emplace_back(name, mat);
        }
        else { GLogError("Uniform '{}' does not exist for this shader.", name); }
    }
    
    void Material::AssignUniformMat3(const std::string &name, const glm::mat3 &mat)
    {
        if (std::find(m_UniformNames.begin(), m_UniformNames.end(), name) != m_UniformNames.end())
        {
            m_UniformSettings.mat3s.emplace_back(name, mat);
        }
        else { GLogError("Uniform '{}' does not exist for this shader.", name); }
    }
    
    void Material::AssignUniformMat4(const std::string &name, const glm::mat4 &mat)
    {
        if (std::find(m_UniformNames.begin(), m_UniformNames.end(), name) != m_UniformNames.end())
        {
            m_UniformSettings.mat4s.emplace_back(name, mat);
        }
        else { GLogError("Uniform '{}' does not exist for this shader.", name); }
    }
    
} // GoombaRender