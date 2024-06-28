#ifndef GOOMBARENDER_INDEX_BUFFER_H
#define GOOMBARENDER_INDEX_BUFFER_H

#include "engine/graphics_context.h"

namespace GoombaRender
{
    class IndexBuffer
    {
    public:
        IndexBuffer(GoombaEngine::GraphicsContext context, size_t count);
        IndexBuffer(GoombaEngine::GraphicsContext context, unsigned int* indices, size_t count);
        virtual ~IndexBuffer();
        
        void Bind() const;
        void Unbind() const;
        
        void SetData(unsigned int* indices, size_t count);
        
        inline size_t GetCount() const { return m_Count; }
    private:
        unsigned int m_RendererID;
        GoombaEngine::GraphicsContext m_Context;
        size_t m_Count;
    };

} // GoombaRender

#endif //GOOMBARENDER_INDEX_BUFFER_H
