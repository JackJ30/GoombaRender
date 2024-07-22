#include "uniform_settings.h"

namespace GoombaRender
{
    void UniformSettings::AssignShader(Shader& shader)
    {
        m_Shader = shader;
    }
    
    void UniformSettings::SetUniforms()
    {
        for (auto& uniform : bools)
        {
            m_Shader.SetUniformBool(uniform.first, uniform.second);
        }
        for (auto& uniform : ints)
        {
            m_Shader.SetUniformInt(uniform.first, uniform.second);
        }
        for (auto& uniform : floats)
        {
            m_Shader.SetUniformFloat(uniform.first, uniform.second);
        }
        for (auto& uniform : vec2s)
        {
            m_Shader.SetUniformVec2(uniform.first, uniform.second);
        }
        for (auto& uniform : vec3s)
        {
            m_Shader.SetUniformVec3(uniform.first, uniform.second);
        }
        for (auto& uniform : vec4s)
        {
            m_Shader.SetUniformVec4(uniform.first, uniform.second);
        }
        for (auto& uniform : mat2s)
        {
            m_Shader.SetUniformMat2(uniform.first, uniform.second);
        }
        for (auto& uniform : mat3s)
        {
            m_Shader.SetUniformMat3(uniform.first, uniform.second);
        }
        for (auto& uniform : mat4s)
        {
            m_Shader.SetUniformMat4(uniform.first, uniform.second);
        }
    }
    
    void UniformSettings::AssignUniformBool(const std::string &name, bool value)
    {
    
    }
    
    void UniformSettings::AssignUniformInt(const std::string &name, int value)
    {
    
    }
    
    void UniformSettings::AssignUniformFloat(const std::string &name, float value)
    {
    
    }
    
    void UniformSettings::AssignUniformVec2(const std::string &name, const glm::vec2 &value)
    {
    
    }
    
    void UniformSettings::AssignUniformVec2(const std::string &name, float x, float y)
    {
    
    }
    
    void UniformSettings::AssignUniformVec3(const std::string &name, const glm::vec3 &value)
    {
    
    }
    
    void UniformSettings::AssignUniformVec3(const std::string &name, float x, float y, float z)
    {
    
    }
    
    void UniformSettings::AssignUniformVec4(const std::string &name, const glm::vec4 &value)
    {
    
    }
    
    void UniformSettings::AssignUniformVec4(const std::string &name, float x, float y, float z, float w)
    {
    
    }
    
    void UniformSettings::AssignUniformMat2(const std::string &name, const glm::mat2 &mat)
    {
    
    }
    
    void UniformSettings::AssignUniformMat3(const std::string &name, const glm::mat3 &mat)
    {
    
    }
    
    void UniformSettings::AssignUniformMat4(const std::string &name, const glm::mat4 &mat)
    {
    
    }
    
    UniformSettings::UniformSettings(Shader test)
        : m_Shader(test)
    {
    
    }
} // GoombaRender