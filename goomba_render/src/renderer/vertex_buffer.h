#ifndef GOOMBARENDER_VERTEX_BUFFER_H
#define GOOMBARENDER_VERTEX_BUFFER_H

#include "engine/graphics_context.h"
#include "renderer/buffer_layout.h"

namespace GoombaRender
{
    class VertexBuffer
    {
    public:
        VertexBuffer(GoombaEngine::GraphicsContext context, size_t size);
        VertexBuffer(GoombaEngine::GraphicsContext context, float* vertices, size_t size);
        ~VertexBuffer();
        
        void Bind() const;
        void Unbind() const;
        
        void SetData(const void* vertices, size_t size);
        
        inline const BufferLayout& GetLayout() const { return m_Layout; }
        inline void SetLayout(const BufferLayout& layout)  { m_Layout = layout; }
    private:
        unsigned int m_RendererID;
        GoombaEngine::GraphicsContext m_Context;
        BufferLayout m_Layout;
    };

} // GoombaRender

#endif //GOOMBARENDER_VERTEX_BUFFER_H
