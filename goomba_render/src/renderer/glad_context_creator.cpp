#include "glad_context_creator.h"

namespace GoombaRender
{
    // OpenGL + GLFW

    void ConfigureSDLOpenGLContext()
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
        GLogInfo("Configured SDL OpenGL Context");
    }

    void RetrieveSDLOpenGLFunctionPtrs(GladGLContext& context)
    {
        if (!gladLoadGLContext(&context, SDL_GL_GetProcAddress))
        {
            GLogCritical("failed to load OpenGL function ptrs");
            return;
        }
        GLogInfo("Retrieved OpenGL SDL function ptrs");
    }

    // Supporting other rendering APIs is not something I am prioritizing at all, but it might be cool to have in the future so I'm (futilely) leaving the door open in some places.
}