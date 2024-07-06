#include "shader.h"
#include <glad/gl.h>
#include <sstream>
#include <fstream>
#include <iostream>

Shader::Shader(const GoombaEngine::GraphicsContext& context, const std::string& filepath) : m_Context(context), m_Filepath(filepath), m_RendererID(0)
{
    CompileShader(ParseShader());
}

Shader::~Shader()
{
    m_Context.GetGlad().DeleteProgram(m_RendererID);
}

ShaderProgramSource Shader::ParseShader()
{
    std::ifstream stream(m_Filepath);

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

    return { ss[0].str(), ss[1].str() };
}

void Shader::CompileShader(const ShaderProgramSource& source) // removed error checking... for now
{
    const char* vertexSource = source.VertexSource.c_str();
    const char* fragmentSource = source.FragmentSource.c_str();
    GLint success;
    
    // Vertex shader
    GLuint vertexShader = m_Context.GetGlad().CreateShader(GL_VERTEX_SHADER);
    m_Context.GetGlad().ShaderSource(vertexShader, 1, &vertexSource, NULL);
    m_Context.GetGlad().CompileShader(vertexShader);

    #ifdef DEBUG
    m_Context.GetGlad().GetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        m_Context.GetGlad().GetShaderInfoLog(vertexShader, 1024, nullptr, m_ErrorMessage);
        GLogError("Vertex shader from '{}' compilation failed:\n{}", m_Filepath, m_ErrorMessage);
    }
    #endif

    // Fragment shader
    GLuint fragmentShader = m_Context.GetGlad().CreateShader(GL_FRAGMENT_SHADER);
    m_Context.GetGlad().ShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    m_Context.GetGlad().CompileShader(fragmentShader);

    #ifdef DEBUG
    m_Context.GetGlad().GetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        m_Context.GetGlad().GetShaderInfoLog(fragmentShader, 1024, nullptr, m_ErrorMessage);
        GLogError("Fragment shader from '{}' compilation failed:\n{}", m_Filepath, m_ErrorMessage);
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
        GLogError("Shader from '{}' failed to link:\n{}", m_Filepath, m_ErrorMessage);
    }
    #endif
    
    // Validation
    m_Context.GetGlad().ValidateProgram(m_RendererID);

    #ifdef DEBUG
    m_Context.GetGlad().GetProgramiv(m_RendererID, GL_VALIDATE_STATUS, &success);
    if(!success)
    {
        m_Context.GetGlad().GetProgramInfoLog(m_RendererID, 1024, nullptr, m_ErrorMessage);
        GLogError("Shader from '{}' failed to validate:\n{}", m_Filepath, m_ErrorMessage);
    }
    #endif
    
    m_Context.GetGlad().DeleteShader(vertexShader);
    m_Context.GetGlad().DeleteShader(fragmentShader);
}

void Shader::Bind() const
{
    m_Context.GetGlad().UseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    m_Context.GetGlad().UseProgram(0);
}

int Shader::GetUniformLocation(const std::string& name)
{
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