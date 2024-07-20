#ifndef GOOMBARENDER_MATERIAL_H
#define GOOMBARENDER_MATERIAL_H

#include "renderer/asset.h"
#include "renderer/shader.h"
#include "renderer/texture.h"

// Materials
// A material has a shader, uniform settings, and textures

namespace GoombaRender
{
    class Material
    {
    
    private:
        Asset<Shader> m_Shader;
        UniformSetting m_UniformSettings;
        std::vector<std::pair<Asset<Texture2D>, unsigned int>> m_Textures;
        
        // flags
    };
    
} // GoombaRender

#endif //GOOMBARENDER_MATERIAL_H
