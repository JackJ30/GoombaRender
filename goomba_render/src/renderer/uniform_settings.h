#ifndef GOOMBARENDER_UNIFORM_SETTINGS_H
#define GOOMBARENDER_UNIFORM_SETTINGS_H

#include "renderer/shader.h"

namespace GoombaRender
{
    // TODO - uniform settings
    // Should take in a shader as reference. It should have functions for "trying" to set a uniform, it will only permit uniforms that the shader actually has. It will have a function to set all the uniforms of the shader
    // TODO - manage reference to the shader better, after the asset rework
    
    class UniformSettings
    {
    public:
        UniformSettings(ShaderInfo test);
        // TODO - fix shader assignment, add checks or redo system
        void AssignShader(ShaderInfo& shader);
        void SetUniforms();
        
        void AssignUniformBool(const std::string &name, bool value);
        void AssignUniformInt(const std::string &name, int value);
        void AssignUniformFloat(const std::string &name, float value);
        void AssignUniformVec2(const std::string &name, const glm::vec2 &value);
        void AssignUniformVec2(const std::string &name, float x, float y);
        void AssignUniformVec3(const std::string &name, const glm::vec3 &value);
        void AssignUniformVec3(const std::string &name, float x, float y, float z);
        void AssignUniformVec4(const std::string &name, const glm::vec4 &value);
        void AssignUniformVec4(const std::string &name, float x, float y, float z, float w);
        void AssignUniformMat2(const std::string &name, const glm::mat2 &mat);
        void AssignUniformMat3(const std::string &name, const glm::mat3 &mat);
        void AssignUniformMat4(const std::string &name, const glm::mat4 &mat);
        
    private:
        ShaderInfo& m_Shader;
        
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
    
} // GoombaRender

#endif //GOOMBARENDER_UNIFORM_SETTINGS_H
