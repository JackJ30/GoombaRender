#ifndef GOOMBARENDER_MATERIAL_H
#define GOOMBARENDER_MATERIAL_H

#include "renderer/asset.h"
#include "renderer/shader.h"

// Materials
// A material has a shader, uniform settings, and textures

namespace GoombaRender
{
    class Material
    {
    private:
        Asset<Shader> m_Shader;
    };
    
} // GoombaRender

#endif //GOOMBARENDER_MATERIAL_H
