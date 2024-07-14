#include "index_buffer.h"

namespace GoombaRender
{
    void IndexBuffer::Create(size_t count)
    {
        RequireContext();
        
        m_Count = count;
        
        m_Context.GetGlad().GenBuffers(1, &m_RendererID);
        m_Context.GetGlad().BindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        m_Context.GetGlad().BufferData(GL_ARRAY_BUFFER, count * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);
        
        m_Created = true;
    }
    
    void IndexBuffer::Create(unsigned int *indices, size_t count)
    {
        RequireContext();
        
        m_Count = count;
        
        m_Context.GetGlad().GenBuffers(1, &m_RendererID);
        m_Context.GetGlad().BindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        m_Context.GetGlad().BufferData(GL_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_DYNAMIC_DRAW);
    
        m_Created = true;
    }
    
    IndexBuffer::~IndexBuffer()
    {
        if (m_HasContext && m_Created)
        {
            m_Context.GetGlad().DeleteBuffers(1, &m_RendererID);
        }
    }
    
    void IndexBuffer::Bind() const
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Index buffer must be created before binding.");
        
        m_Context.GetGlad().BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }
    
    void IndexBuffer::Unbind() const
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Index buffer must be created before unbinding.");
        
        m_Context.GetGlad().BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    
    void IndexBuffer::SetData(unsigned int *indices, size_t count)
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Index buffer must be created before setting data.");
        
        m_Count = count;
        m_Context.GetGlad().BindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        m_Context.GetGlad().BufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(unsigned int), indices);
    }
} // GoombaRender