#ifndef GLAD_CONTEXT_CREATOR_H
#define GLAD_CONTEXT_CREATOR_H

#include "goombapch.h"

#include <glad/gl.h>
#include <SDL3/SDL.h>

namespace GoombaRender
{
    void ConfigureSDLOpenGLContext();
    void RetrieveSDLOpenGLFunctionPtrs(GladGLContext& context);
}

#endif // GLAD_CONTEXT_CREATOR_H