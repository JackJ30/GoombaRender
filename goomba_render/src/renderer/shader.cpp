#include "shader.h"
#include <glad/gl.h>

namespace GoombaRender
{
    void Shader::Create(const std::string &vertexSource, const std::string &fragmentSource)
    {
        RequireContext();
        
        const char* vertexSrc = vertexSource.c_str();
        const char* fragmentSrc = fragmentSource.c_str();
        GLint success;
        
        // Vertex shader
        GLuint vertexShader = m_Context.GetGlad().CreateShader(GL_VERTEX_SHADER);
        m_Context.GetGlad().ShaderSource(vertexShader, 1, &vertexSrc, NULL);
        m_Context.GetGlad().CompileShader(vertexShader);

#ifdef DEBUG
        m_Context.GetGlad().GetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            m_Context.GetGlad().GetShaderInfoLog(vertexShader, 1024, nullptr, m_ErrorMessage);
            GLogError("Vertex shader compilation failed:\n{}", m_ErrorMessage);
        }
#endif
        
        // Fragment shader
        GLuint fragmentShader = m_Context.GetGlad().CreateShader(GL_FRAGMENT_SHADER);
        m_Context.GetGlad().ShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
        m_Context.GetGlad().CompileShader(fragmentShader);

#ifdef DEBUG
        m_Context.GetGlad().GetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            m_Context.GetGlad().GetShaderInfoLog(fragmentShader, 1024, nullptr, m_ErrorMessage);
            GLogError("Fragment shader compilation failed:\n{}", m_ErrorMessage);
        }
#endif
        
        // Program and linking
        m_RendererID = m_Context.GetGlad().CreateProgram();
        m_Context.GetGlad().AttachShader(m_RendererID, vertexShader);
        m_Context.GetGlad().AttachShader(m_RendererID, fragmentShader);
        m_Context.GetGlad().LinkProgram(m_RendererID);

#ifdef DEBUG
        m_Context.GetGlad().GetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
        if(!success)
        {
            m_Context.GetGlad().GetProgramInfoLog(m_RendererID, 1024, nullptr, m_ErrorMessage);
            GLogError("Shader from failed to link:\n{}", m_ErrorMessage);
        }
#endif
        
        // Validation
        m_Context.GetGlad().ValidateProgram(m_RendererID);

#ifdef DEBUG
        m_Context.GetGlad().GetProgramiv(m_RendererID, GL_VALIDATE_STATUS, &success);
        if(!success)
        {
            m_Context.GetGlad().GetProgramInfoLog(m_RendererID, 1024, nullptr, m_ErrorMessage);
            GLogError("Shader from '{}' failed to validate:\n{}", m_ErrorMessage);
        }
#endif
        
        m_Context.GetGlad().DeleteShader(vertexShader);
        m_Context.GetGlad().DeleteShader(fragmentShader);
        
        m_Created = true;
    }
    
    void Shader::Bind() const
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Shader must be created before binding.");
        
        m_Context.GetGlad().UseProgram(m_RendererID);
    }
    
    void Shader::Unbind() const
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Shader must be created before binding.");
        
        m_Context.GetGlad().UseProgram(0);
    }
    
    void Shader::Delete()
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Shader must be created before using uniforms.");
        
        m_Context.GetGlad().DeleteProgram(m_RendererID);
    }
    
    int Shader::GetUniformLocation(const std::string& name)
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Shader must be created before using uniforms.");
        
        if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
            return m_UniformLocationCache[name];
        
        int location = m_Context.GetGlad().GetUniformLocation(m_RendererID, name.c_str());
        if (location == -1) std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
        
        m_UniformLocationCache[name] = location;
        return location;
    }
    
    void Shader::SetUniformBool(const std::string &name, bool value)
    {
        m_Context.GetGlad().Uniform1i(GetUniformLocation(name), (int)value);
    }
    
    void Shader::SetUniformInt(const std::string &name, int value)
    {
        m_Context.GetGlad().Uniform1i(GetUniformLocation(name), value);
    }
    
    void Shader::SetUniformFloat(const std::string &name, float value)
    {
        m_Context.GetGlad().Uniform1f(GetUniformLocation(name), value);
    }
    
    void Shader::SetUniformVec2(const std::string &name, const glm::vec2 &value)
    {
        m_Context.GetGlad().Uniform2fv(GetUniformLocation(name), 1, &value[0]);
    }
    
    void Shader::SetUniformVec2(const std::string &name, float x, float y)
    {
        m_Context.GetGlad().Uniform2f(GetUniformLocation(name), x, y);
    }
    
    void Shader::SetUniformVec3(const std::string &name, const glm::vec3 &value)
    {
        m_Context.GetGlad().Uniform3fv(GetUniformLocation(name), 1, &value[0]);
    }
    
    void Shader::SetUniformVec3(const std::string &name, float x, float y, float z)
    {
        m_Context.GetGlad().Uniform3f(GetUniformLocation(name), x, y, z);
    }
    
    void Shader::SetUniformVec4(const std::string &name, const glm::vec4 &value)
    {
        m_Context.GetGlad().Uniform4fv(GetUniformLocation(name), 1, &value[0]);
    }
    
    void Shader::SetUniformVec4(const std::string &name, float x, float y, float z, float w)
    {
        m_Context.GetGlad().Uniform4f(GetUniformLocation(name), x, y, z, w);
    }
    
    void Shader::SetUniformMat2(const std::string &name, const glm::mat2 &mat)
    {
        m_Context.GetGlad().UniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    
    void Shader::SetUniformMat3(const std::string &name, const glm::mat3 &mat)
    {
        m_Context.GetGlad().UniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    
    void Shader::SetUniformMat4(const std::string &name, const glm::mat4 &mat)
    {
        m_Context.GetGlad().UniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    
    void Shader::SetUniforms(const UniformSetting &setting)
    {
        for (auto& uniform : setting.bools)
        {
            SetUniformBool(uniform.first, uniform.second);
        }
        for (auto& uniform : setting.ints)
        {
            SetUniformInt(uniform.first, uniform.second);
        }
        for (auto& uniform : setting.floats)
        {
            SetUniformFloat(uniform.first, uniform.second);
        }
        for (auto& uniform : setting.vec2s)
        {
            SetUniformVec2(uniform.first, uniform.second);
        }
        for (auto& uniform : setting.vec3s)
        {
            SetUniformVec3(uniform.first, uniform.second);
        }
        for (auto& uniform : setting.vec4s)
        {
            SetUniformVec4(uniform.first, uniform.second);
        }
        for (auto& uniform : setting.mat2s)
        {
            SetUniformMat2(uniform.first, uniform.second);
        }
        for (auto& uniform : setting.mat3s)
        {
            SetUniformMat3(uniform.first, uniform.second);
        }
        for (auto& uniform : setting.mat4s)
        {
            SetUniformMat4(uniform.first, uniform.second);
        }
    }
    
    void LoadShader(Asset<Shader>& asset, GoombaEngine::GraphicsContext& context)
    {
        std::ifstream stream(asset.GetPath());
        
        enum class ShaderType
        {
            NONE = 1, VERTEX = 0, FRAGMENT = 1
        };
        
        std::string line;
        std::stringstream ss[2];
        ShaderType type = ShaderType::NONE;
        while (getline(stream, line))
        {
            if (line.find("#shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                    type = ShaderType::VERTEX;
                else if (line.find("fragment") != std::string::npos)
                    type = ShaderType::FRAGMENT;
            }
            else
            {
                ss[(int)type] << line << "\n";
            }
        }
        
        Shader shader;
        shader.AssignContext(context);
        shader.Create(ss[0].str(), ss[1].str());
        
        asset.AssignLoaded(std::move(shader));
    }
}