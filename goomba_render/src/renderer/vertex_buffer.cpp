#include "vertex_buffer.h"

namespace GoombaRender
{
    VertexBuffer::VertexBuffer(GoombaEngine::GraphicsContext context, size_t size)
        : m_Context(context)
    {
        m_Context.GetGlad().GenBuffers(1, &m_RendererID);
        Bind();
        m_Context.GetGlad().BufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }
    
    VertexBuffer::VertexBuffer(GoombaEngine::GraphicsContext context, float *vertices, size_t size)
        : m_Context(context)
    {
        m_Context.GetGlad().GenBuffers(1, &m_RendererID);
        Bind();
        m_Context.GetGlad().BufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }
    
    VertexBuffer::~VertexBuffer()
    {
        m_Context.GetGlad().DeleteBuffers(1, &m_RendererID);
    }
    
    void VertexBuffer::Bind() const
    {
        m_Context.GetGlad().BindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }
    
    void VertexBuffer::Unbind() const
    {
        m_Context.GetGlad().BindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    void VertexBuffer::SetData(const void *vertices, size_t size)
    {
        Bind();
        m_Context.GetGlad().BufferSubData(GL_ARRAY_BUFFER, 0, size, vertices);
    }
} // GoombaRender