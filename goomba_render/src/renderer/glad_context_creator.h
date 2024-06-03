#ifndef GLAD_CONTEXT_CREATOR_H
#define GLAD_CONTEXT_CREATOR_H

#include <glad/gl.h>
#include <SDL3/SDL.h>

namespace GoombaRender
{
    void ConfigureSDLOpenGLContext();
    void CreateSDLOpenGLContext(SDL_Window* window);
}

#endif // GLAD_CONTEXT_CREATOR_H