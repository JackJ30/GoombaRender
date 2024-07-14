#include "vertex_buffer.h"

namespace GoombaRender
{
    void VertexBuffer::Create(size_t size)
    {
        RequireContext();
        
        m_Context.GetGlad().GenBuffers(1, &m_RendererID);
        m_Context.GetGlad().BindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        m_Context.GetGlad().BufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
        
        m_Created = true;
    }
    
    void VertexBuffer::Create(float *vertices, size_t size)
    {
        RequireContext();
        
        m_Context.GetGlad().GenBuffers(1, &m_RendererID);
        m_Context.GetGlad().BindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        m_Context.GetGlad().BufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        
        m_Created = true;
    }
    
    VertexBuffer::~VertexBuffer()
    {
        if (m_HasContext && m_Created)
        {
            m_Context.GetGlad().DeleteBuffers(1, &m_RendererID);
        }
    }
    
    void VertexBuffer::Bind() const
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Vertex buffer must be created before binding.");
        
        m_Context.GetGlad().BindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }
    
    void VertexBuffer::Unbind() const
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Vertex buffer must be created before unbinding.");
        
        m_Context.GetGlad().BindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    void VertexBuffer::SetData(const void *vertices, size_t size)
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Vertex buffer must be created before setting data.");
        
        m_Context.GetGlad().BindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        m_Context.GetGlad().BufferSubData(GL_ARRAY_BUFFER, 0, size, vertices);
    }
} // GoombaRender