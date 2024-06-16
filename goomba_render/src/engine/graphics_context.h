#ifndef GOOMBARENDER_GRAPHICS_CONTEXT_H
#define GOOMBARENDER_GRAPHICS_CONTEXT_H

#include "glad/gl.h"

namespace GoombaEngine {

    class GraphicsContext {
    public:
        void CreateContext(GLADloadfunc func);

        inline GladGLContext& GetGladContext() { return m_GladContext; }
    private:
        GladGLContext m_GladContext;
    };

} // GoombaEngine

#endif //GOOMBARENDER_GRAPHICS_CONTEXT_H
