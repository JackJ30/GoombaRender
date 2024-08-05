#ifndef GOOMBARENDER_MATERIAL_H
#define GOOMBARENDER_MATERIAL_H

#include "renderer/uniform_settings.h"
#include "renderer/shader.h"
#include "renderer/texture.h"

// Materials
// A material has a shader, uniform settings, and textures

// TODO - make more user friendly method of setting uniforms
// TODO - make render flags

namespace GoombaRender
{
    class Material
    {
    public:
        Material(std::shared_ptr<ShaderInfo> shader);
    
        void Bind() const;
        
        inline bool HasUniform(const std::string& name) const { return std::find(m_UniformNames.begin(),
                                                                                 m_UniformNames.end(), name) != m_UniformNames.end(); }
        inline UniformSettings& GetUniformSettings() { return m_UniformSettings; }
        
        void AssignUniformTexture(const std::string& name, std::shared_ptr<Texture2DInfo> texture);
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
        
        // flags
        bool m_DoubleSided = false;
    private:
        std::shared_ptr<ShaderInfo> m_Shader;
        UniformSettings m_UniformSettings;
        
        std::unordered_map<std::string, std::shared_ptr<Texture2DInfo>> m_Textures;
        std::unordered_set<std::string> m_UnassignedTextures;
        std::vector<std::string> m_UniformNames;
    };
    
} // GoombaRender

#endif //GOOMBARENDER_MATERIAL_H
