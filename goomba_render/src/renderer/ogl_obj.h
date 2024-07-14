#ifndef GOOMBARENDER_OGL_OBJ_H
#define GOOMBARENDER_OGL_OBJ_H

#include "engine/graphics_context.h"

namespace GoombaRender
{
    class OglObj
    {
    public:
        void AssignContext(GoombaEngine::GraphicsContext& context) { m_Context = context; m_HasContext = true; } // TODO - should manage this reference better
        inline bool IsCreated() const { return m_Created; }
        
    protected:
        OglObj() = default;
        inline void RequireContext() const { DEBUG_ASSERT(m_HasContext == true, "Context must be assigned before using OGL object."); }
        
        GoombaEngine::GraphicsContext m_Context;
        bool m_HasContext = false;
        bool m_Created = false;
    };
    
} // GoombaRender

#endif //GOOMBARENDER_OGL_OBJ_H
