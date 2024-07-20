#ifndef GOOMBARENDER_MATERIAL_H
#define GOOMBARENDER_MATERIAL_H

#include "renderer/asset.h"
#include "renderer/shader.h"
#include "renderer/texture.h"

// Materials
// A material has a shader, uniform settings, and textures

// TODO - make more user friendly method of setting uniforms
// TODO - make render flags
// TODO - system to "try" to set a uniform if it exists

namespace GoombaRender
{
    class Material : OglObj
    {
    public:
        void Create(Asset<Shader> shader);
    
        void Bind() const;
        
        inline UniformSetting& GetUniformSettings() { return m_UniformSettings; }
        
        void AssignTextureUniform(std::string name, Asset<Texture2D> texture);
        inline const std::vector<Asset<Texture2D>>& GetTextures() const { return m_Textures; }
        
        // flags
        
    private:
        Asset<Shader> m_Shader;
        UniformSetting m_UniformSettings;
        std::vector<Asset<Texture2D>> m_Textures;
    };
    
} // GoombaRender

#endif //GOOMBARENDER_MATERIAL_H
