#ifndef GOOMBARENDER_SHADER_H
#define GOOMBARENDER_SHADER_H

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include "engine/graphics_context.h"

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
public:
    Shader(const GoombaEngine::GraphicsContext& context, const std::string& filepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // Set uniforms
    void SetUniformBool(const std::string &name, bool value);
    void SetUniformInt(const std::string &name, int value);
    void SetUniformFloat(const std::string &name, float value);
    void SetUniformVec2(const std::string &name, const glm::vec2 &value);
    void SetUniformVec2(const std::string &name, float x, float y);
    void SetUniformVec3(const std::string &name, const glm::vec3 &value);
    void SetUniformVec3(const std::string &name, float x, float y, float z);
    void SetUniformVec4(const std::string &name, const glm::vec4 &value);
    void SetUniformVec4(const std::string &name, float x, float y, float z, float w);
    void SetUniformMat2(const std::string &name, const glm::mat2 &mat);
    void SetUniformMat3(const std::string &name, const glm::mat3 &mat);
    void SetUniformMat4(const std::string &name, const glm::mat4 &mat);
private:
    GoombaEngine::GraphicsContext m_Context;
    std::string m_Filepath;
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;
    GLchar m_ErrorMessage[1024];
    
    ShaderProgramSource ParseShader();
    void CompileShader(const ShaderProgramSource& source);
    int GetUniformLocation(const std::string& name);
};

#endif