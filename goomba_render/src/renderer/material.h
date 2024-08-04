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
        void Create(std::shared_ptr<ShaderInfo> shader);
    
        void Bind() const;
        
        inline UniformSettings& GetUniformSettings() { return m_UniformSettings; }
        
        void AssignTextureUniform(const std::string& name, const Asset<Texture2DInfo>& texture);
        
        // flags
        
    private:
        std::shared_ptr<ShaderInfo> m_Shader;
        UniformSettings m_UniformSettings;
        std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_Textures;
        std::unordered_set<std::string> m_UnassignedTextures;
    };
    
} // GoombaRender

#endif //GOOMBARENDER_MATERIAL_H
