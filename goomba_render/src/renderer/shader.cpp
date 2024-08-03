#include "shader.h"
#include <glad/gl.h>

namespace GoombaRender
{
    ShaderInfo::ShaderInfo(unsigned int rendererID)
        : rendererID(rendererID)
    {
    
    }
    
    void ShaderInfo::CompileAndLink(const std::string &vertexSource, const std::string &fragmentSource)
    {
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
            glad.GetShaderInfoLog(vertexShader, 1024, nullptr, errorMessage);
            GLogError("Vertex shader compilation failed:\n{}", errorMessage);
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
            glad.GetShaderInfoLog(fragmentShader, 1024, nullptr, errorMessage);
            GLogError("Fragment shader compilation failed:\n{}", errorMessage);
        }
#endif
        
        // Program and linking
        glad.AttachShader(rendererID, vertexShader);
        glad.AttachShader(rendererID, fragmentShader);
        glad.LinkProgram(rendererID);

#ifdef DEBUG
        glad.GetProgramiv(rendererID, GL_LINK_STATUS, &success);
        if(!success)
        {
            glad.GetProgramInfoLog(rendererID, 1024, nullptr, errorMessage);
            GLogError("ShaderInfo from failed to link:\n{}", errorMessage);
        }
#endif
        
        // Validation
        glad.ValidateProgram(rendererID);

#ifdef DEBUG
        glad.GetProgramiv(rendererID, GL_VALIDATE_STATUS, &success);
        if(!success)
        {
            glad.GetProgramInfoLog(rendererID, 1024, nullptr, errorMessage);
            GLogError("ShaderInfo from '{}' failed to validate:\n{}", errorMessage);
        }
#endif
        
        glad.DeleteShader(vertexShader);
        glad.DeleteShader(fragmentShader);
        
        uniformsCache.clear();
        uniformLocationCache.clear();
        
        // Cache uniforms
        int count;
        glad.GetProgramiv(rendererID, GL_ACTIVE_UNIFORMS, &count);
        uniformsCache.reserve(count);
        
        const GLsizei bufSize = 64;
        GLchar name[bufSize];
        GLsizei length;
        GLint size;
        GLenum type;
        
        for (int i = 0; i < count; ++i)
        {
            glad.GetActiveUniform(rendererID, i, bufSize, &length, &size, &type, name);
            uniformsCache.emplace_back(type, name);
        }
    }
    
    int ShaderInfo::GetUniformLocation(const std::string& name)
    {
        if (uniformLocationCache.find(name) != uniformLocationCache.end())
            return uniformLocationCache[name];
        
        int location = glad.GetUniformLocation(rendererID, name.c_str());
        if (location == -1) std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
        
        uniformLocationCache[name] = location;
        return location;
    }
    
    void ShaderInfo::SetUniformBool(const std::string &name, bool value)
    {
        glad.Uniform1i(GetUniformLocation(name), (int)value);
    }
    
    void ShaderInfo::SetUniformInt(const std::string &name, int value)
    {
        glad.Uniform1i(GetUniformLocation(name), value);
    }
    
    void ShaderInfo::SetUniformFloat(const std::string &name, float value)
    {
        glad.Uniform1f(GetUniformLocation(name), value);
    }
    
    void ShaderInfo::SetUniformVec2(const std::string &name, const glm::vec2 &value)
    {
        glad.Uniform2fv(GetUniformLocation(name), 1, &value[0]);
    }
    
    void ShaderInfo::SetUniformVec2(const std::string &name, float x, float y)
    {
        glad.Uniform2f(GetUniformLocation(name), x, y);
    }
    
    void ShaderInfo::SetUniformVec3(const std::string &name, const glm::vec3 &value)
    {
        glad.Uniform3fv(GetUniformLocation(name), 1, &value[0]);
    }
    
    void ShaderInfo::SetUniformVec3(const std::string &name, float x, float y, float z)
    {
        glad.Uniform3f(GetUniformLocation(name), x, y, z);
    }
    
    void ShaderInfo::SetUniformVec4(const std::string &name, const glm::vec4 &value)
    {
        glad.Uniform4fv(GetUniformLocation(name), 1, &value[0]);
    }
    
    void ShaderInfo::SetUniformVec4(const std::string &name, float x, float y, float z, float w)
    {
        glad.Uniform4f(GetUniformLocation(name), x, y, z, w);
    }
    
    void ShaderInfo::SetUniformMat2(const std::string &name, const glm::mat2 &mat)
    {
        glad.UniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    
    void ShaderInfo::SetUniformMat3(const std::string &name, const glm::mat3 &mat)
    {
        glad.UniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    
    void ShaderInfo::SetUniformMat4(const std::string &name, const glm::mat4 &mat)
    {
        glad.UniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    
    ShaderInfo CreateShader(const std::string &vertexSource, const std::string &fragmentSource)
    {
        ShaderInfo shader(glad.CreateProgram());
        shader.CompileAndLink(vertexSource, fragmentSource);
        return shader;
    }
}