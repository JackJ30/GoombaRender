//
// Created by jack on 6/22/24.
//

#include "vertex_array.h"

namespace GoombaRender
{
    void VertexArray::Create()
    {
        RequireContext();
        
        m_Context.GetGlad().GenVertexArrays(1, &m_RendererID);
        m_Created = true;
    }
    
    void VertexArray::Delete()
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Vertex array must be created before deleting.");
        
        m_Context.GetGlad().DeleteVertexArrays(1, &m_RendererID);
        
        m_Context.GetGlad().DeleteBuffers(m_VertexBuffers.size(), m_VertexBuffers.data());
        m_Context.GetGlad().DeleteBuffers(1, &m_IndexBuffer);
    }
    
    void VertexArray::Bind() const
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Vertex array must be created before binding.");
        
        m_Context.GetGlad().BindVertexArray(m_RendererID);
    }
    
    void VertexArray::Unbind() const
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Vertex array must be created before unbinding.");
        
        m_Context.GetGlad().BindVertexArray(0);
    }
    
    void VertexArray::AddVertexBuffer(float* vertices, size_t size, const BufferLayout& layout)
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Vertex array must be created before adding vertex buffer.");
        
        m_Context.GetGlad().BindVertexArray(m_RendererID);
        
        unsigned int vbo;
        m_Context.GetGlad().GenBuffers(1, &vbo);
        m_Context.GetGlad().BindBuffer(GL_ARRAY_BUFFER, vbo);
        m_Context.GetGlad().BufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        
        size_t index = 0;
        for (const BufferElement& element : layout)
        {
            m_Context.GetGlad().EnableVertexAttribArray(index);
            m_Context.GetGlad().VertexAttribPointer(index, element.GetComponentCount(), element.GetGLType(), element.Normalized, layout.GetStride(), (const void*)element.Offset);
            index++;
        }
        
        m_VertexBuffers.push_back(vbo);
    }
    
    void VertexArray::SetIndexBuffer(unsigned int *indices, size_t numIndices)
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Vertex array must be created before setting index buffer.");
        
        m_Context.GetGlad().BindVertexArray(m_RendererID);
        
        unsigned int ibo;
        m_Context.GetGlad().GenBuffers(1, &ibo);
        m_Context.GetGlad().BindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        m_Context.GetGlad().BufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
        
        m_NumIndices = numIndices;
        m_IndexBuffer = ibo;
    }
} // GoombaRender