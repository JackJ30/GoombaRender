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
        GLuint vertexShader = glad.CreateShader(GL_VERTEX_SHADER);
        glad.ShaderSource(vertexShader, 1, &vertexSrc, NULL);
        glad.CompileShader(vertexShader);

#ifdef DEBUG
        glad.GetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glad.GetShaderInfoLog(vertexShader, 1024, nullptr, m_ErrorMessage);
            GLogError("Vertex shader compilation failed:\n{}", m_ErrorMessage);
        }
#endif
        
        // Fragment shader
        GLuint fragmentShader = glad.CreateShader(GL_FRAGMENT_SHADER);
        glad.ShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
        glad.CompileShader(fragmentShader);

#ifdef DEBUG
        glad.GetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glad.GetShaderInfoLog(fragmentShader, 1024, nullptr, m_ErrorMessage);
            GLogError("Fragment shader compilation failed:\n{}", m_ErrorMessage);
        }
#endif
        
        // Program and linking
        m_RendererID = glad.CreateProgram();
        glad.AttachShader(m_RendererID, vertexShader);
        glad.AttachShader(m_RendererID, fragmentShader);
        glad.LinkProgram(m_RendererID);

#ifdef DEBUG
        glad.GetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
        if(!success)
        {
            glad.GetProgramInfoLog(m_RendererID, 1024, nullptr, m_ErrorMessage);
            GLogError("Shader from failed to link:\n{}", m_ErrorMessage);
        }
#endif
        
        // Validation
        glad.ValidateProgram(m_RendererID);

#ifdef DEBUG
        glad.GetProgramiv(m_RendererID, GL_VALIDATE_STATUS, &success);
        if(!success)
        {
            glad.GetProgramInfoLog(m_RendererID, 1024, nullptr, m_ErrorMessage);
            GLogError("Shader from '{}' failed to validate:\n{}", m_ErrorMessage);
        }
#endif
        
        glad.DeleteShader(vertexShader);
        glad.DeleteShader(fragmentShader);
        
        // Cache uniforms
        int count;
        glad.GetProgramiv(m_RendererID, GL_ACTIVE_UNIFORMS, &count);
        m_UniformsCache.reserve(count);
        
        const GLsizei bufSize = 64;
        GLchar name[bufSize];
        GLsizei length;
        GLint size;
        GLenum type;
        
        for (int i = 0; i < count; ++i)
        {
            glad.GetActiveUniform(m_RendererID, i, bufSize, &length, &size, &type, name);
            m_UniformsCache.emplace_back(type, name);
        }
        
        m_Created = true;
    }
    
    void Shader::Bind() const
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Shader must be created before binding.");
        
        glad.UseProgram(m_RendererID);
    }
    
    void Shader::Unbind() const
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Shader must be created before binding.");
        
        glad.UseProgram(0);
    }
    
    void Shader::Delete()
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Shader must be created before using uniforms.");
        
        glad.DeleteProgram(m_RendererID);
    }
    
    int Shader::GetUniformLocation(const std::string& name)
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Shader must be created before using uniforms.");
        
        if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
            return m_UniformLocationCache[name];
        
        int location = glad.GetUniformLocation(m_RendererID, name.c_str());
        if (location == -1) std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
        
        m_UniformLocationCache[name] = location;
        return location;
    }
    
    void Shader::SetUniformBool(const std::string &name, bool value)
    {
        glad.Uniform1i(GetUniformLocation(name), (int)value);
    }
    
    void Shader::SetUniformInt(const std::string &name, int value)
    {
        glad.Uniform1i(GetUniformLocation(name), value);
    }
    
    void Shader::SetUniformFloat(const std::string &name, float value)
    {
        glad.Uniform1f(GetUniformLocation(name), value);
    }
    
    void Shader::SetUniformVec2(const std::string &name, const glm::vec2 &value)
    {
        glad.Uniform2fv(GetUniformLocation(name), 1, &value[0]);
    }
    
    void Shader::SetUniformVec2(const std::string &name, float x, float y)
    {
        glad.Uniform2f(GetUniformLocation(name), x, y);
    }
    
    void Shader::SetUniformVec3(const std::string &name, const glm::vec3 &value)
    {
        glad.Uniform3fv(GetUniformLocation(name), 1, &value[0]);
    }
    
    void Shader::SetUniformVec3(const std::string &name, float x, float y, float z)
    {
        glad.Uniform3f(GetUniformLocation(name), x, y, z);
    }
    
    void Shader::SetUniformVec4(const std::string &name, const glm::vec4 &value)
    {
        glad.Uniform4fv(GetUniformLocation(name), 1, &value[0]);
    }
    
    void Shader::SetUniformVec4(const std::string &name, float x, float y, float z, float w)
    {
        glad.Uniform4f(GetUniformLocation(name), x, y, z, w);
    }
    
    void Shader::SetUniformMat2(const std::string &name, const glm::mat2 &mat)
    {
        glad.UniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    
    void Shader::SetUniformMat3(const std::string &name, const glm::mat3 &mat)
    {
        glad.UniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    
    void Shader::SetUniformMat4(const std::string &name, const glm::mat4 &mat)
    {
        glad.UniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    
    void LoadShader(Asset<Shader>& asset, GoombaEngine::GraphicsContext& context)
    {
        if (asset.TryLoadFromCache()) return;
        if (!asset.GetPath().has_value()) { GLogError("Can not load shader with no path."); return; }
        
        std::ifstream stream(asset.GetPath().value());
        
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