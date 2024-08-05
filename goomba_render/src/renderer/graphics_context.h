#ifndef GOOMBARENDER_GRAPHICS_CONTEXT_H
#define GOOMBARENDER_GRAPHICS_CONTEXT_H

#include "glad/gl.h"

namespace GoombaRender
{
    inline GladGLContext glad;
    
    void LoadGraphicsContext(GLADloadfunc func);
} // GoombaRender

#endif //GOOMBARENDER_GRAPHICS_CONTEXT_H
