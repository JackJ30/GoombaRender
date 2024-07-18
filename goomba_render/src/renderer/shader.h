#ifndef GOOMBARENDER_SHADER_H
#define GOOMBARENDER_SHADER_H

#include "renderer/ogl_obj.h"

#include "renderer/asset.h"

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

#include <sstream>
#include <fstream>

namespace GoombaRender
{
    struct UniformSetting
    {
        std::vector<std::pair<std::string, bool>> bools;
        std::vector<std::pair<std::string, int>> ints;
        std::vector<std::pair<std::string, float>> floats;
        std::vector<std::pair<std::string, glm::vec2>> vec2s;
        std::vector<std::pair<std::string, glm::vec3>> vec3s;
        std::vector<std::pair<std::string, glm::vec4>> vec4s;
        std::vector<std::pair<std::string, glm::mat2>> mat2s;
        std::vector<std::pair<std::string, glm::mat3>> mat3s;
        std::vector<std::pair<std::string, glm::mat4>> mat4s;
    };
    
    class Shader : public OglObj
    {
    public:
        Shader() = default;
    
        void Create(const std::string &vertexSource, const std::string &fragmentSource);
        void Delete();
        
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
        void SetUniforms(const UniformSetting& setting);
    private:
        unsigned int m_RendererID;
        
        std::unordered_map<std::string, int> m_UniformLocationCache;
        GLchar m_ErrorMessage[1024];
        
        int GetUniformLocation(const std::string& name);
    };
    
    void LoadShader(Asset<Shader>& asset, GoombaEngine::GraphicsContext& context);
}

#endif