#ifndef GOOMBARENDER_MATERIAL_H
#define GOOMBARENDER_MATERIAL_H

#include "renderer/asset.h"
#include "renderer/uniform_settings.h"
#include "renderer/shader.h"
#include "renderer/texture.h"

// Materials
// A material has a shader, uniform settings, and textures

// TODO - make more user friendly method of setting uniforms
// TODO - make render flags

namespace GoombaRender
{
    class Material : public OglObj
    {
    public:
        void Create(Asset<Shader> shader);
    
        void Bind();
        
        inline UniformSettings& GetUniformSettings() { return m_UniformSettings; }
        
        void AssignTextureUniform(const std::string& name, const Asset<Texture2D>& texture);
        
        // flags
        
    private:
        Asset<Shader> m_Shader;
        UniformSettings m_UniformSettings;
        std::unordered_map<std::string, Asset<Texture2D>> m_Textures;
        std::unordered_set<std::string> m_UnassignedTextures;
    };
    
} // GoombaRender

#endif //GOOMBARENDER_MATERIAL_H
