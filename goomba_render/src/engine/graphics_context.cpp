//
// Created by jack on 6/15/24.
//

#include "graphics_context.h"

namespace GoombaEngine {
    void GraphicsContext::CreateContext(GLADloadfunc func) {
        GLogInfo("Loading OpenGL context...");
        if (!gladLoadGLContext(&m_GladContext, func))
        {
            GLogCritical("failed to load OpenGL function ptrs");
            return;
        }
        GLogTrace("OpenGL context loaded.");
    }
} // GoombaEngine