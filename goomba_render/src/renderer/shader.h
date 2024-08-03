#ifndef GOOMBARENDER_SHADER_H
#define GOOMBARENDER_SHADER_H

#include <glm/glm.hpp>

#include "renderer/graphics_context.h"

namespace GoombaRender
{
    struct ShaderInfo
    {
    public:
        explicit ShaderInfo(unsigned int rendererID);
        
        ShaderInfo(const ShaderInfo&) = delete;
        ShaderInfo& operator=(const ShaderInfo&) = delete;
        ShaderInfo(ShaderInfo&&) = default;
        ShaderInfo& operator=(ShaderInfo&&) = default;
        
        inline void Delete() { glad.DeleteProgram(rendererID); }
        
        void CompileAndLink(const std::string &vertexSource, const std::string &fragmentSource);
        inline void Bind() { glad.UseProgram(rendererID); }
        
        inline unsigned int GetRendererID() const { return rendererID; }
        inline const std::vector<std::pair<GLenum, std::string>>& GetUniforms() const { return uniformsCache; }
    
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
        unsigned int rendererID;
        
        std::unordered_map<std::string, int> uniformLocationCache;
        std::vector<std::pair<GLenum, std::string>> uniformsCache;
        GLchar errorMessage[1024];
        
        int GetUniformLocation(const std::string& name);
    };
    
    ShaderInfo CreateShader(const std::string &vertexSource, const std::string &fragmentSource);
}

#endif