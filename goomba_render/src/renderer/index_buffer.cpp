#include "index_buffer.h"

namespace GoombaRender
{
    IndexBuffer::IndexBuffer(GoombaEngine::GraphicsContext context, size_t count)
        : m_Context(context), m_Count(count)
    {
        m_Context.GetGlad().GenBuffers(1, &m_RendererID);
        m_Context.GetGlad().BindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        m_Context.GetGlad().BufferData(GL_ARRAY_BUFFER, count * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);
    }
    
    IndexBuffer::IndexBuffer(GoombaEngine::GraphicsContext context, unsigned int *indices, size_t count)
        : m_Context(context), m_Count(count)
    {
        m_Context.GetGlad().GenBuffers(1, &m_RendererID);
        m_Context.GetGlad().BindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        m_Context.GetGlad().BufferData(GL_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_DYNAMIC_DRAW);
    }
    
    IndexBuffer::~IndexBuffer()
    {
        m_Context.GetGlad().DeleteBuffers(1, &m_RendererID);
    }
    
    void IndexBuffer::Bind() const
    {
        m_Context.GetGlad().BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }
    
    void IndexBuffer::Unbind() const
    {
        m_Context.GetGlad().BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    
    void IndexBuffer::SetData(unsigned int *indices, size_t count)
    {
        m_Count = count;
        m_Context.GetGlad().BindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        m_Context.GetGlad().BufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(unsigned int), indices);
    }
} // GoombaRender