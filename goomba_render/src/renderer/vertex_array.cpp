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
    
    VertexArray::~VertexArray()
    {
        if (m_HasContext && m_Created)
        {
            m_Context.GetGlad().DeleteVertexArrays(1, &m_RendererID);
        }
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
    
    void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer)
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Vertex array must be created before adding vertex buffer.");
        DEBUG_ASSERT(vertexBuffer->IsCreated(), "Vertex buffer must be created before adding to vertex array.");
        ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout");
        
        m_Context.GetGlad().BindVertexArray(m_RendererID);
        vertexBuffer->Bind();
        
        size_t index = 0;
        const BufferLayout& layout = vertexBuffer->GetLayout();
        for (const BufferElement& element : layout)
        {
            m_Context.GetGlad().EnableVertexAttribArray(index);
            m_Context.GetGlad().VertexAttribPointer(index, element.GetComponentCount(), element.GetGLType(), element.Normalized, layout.GetStride(), (const void*)element.Offset);
            index++;
        }
        
        m_VertexBuffers.push_back(vertexBuffer);
    }
    
    void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> indexBuffer)
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Vertex array must be created before setting index buffer.");
        DEBUG_ASSERT(indexBuffer->IsCreated(), "Index buffer must be created before adding to vertex array.");
        
        m_Context.GetGlad().BindVertexArray(m_RendererID);
        indexBuffer->Bind();
        
        m_IndexBuffer = indexBuffer;
    }
    
    unsigned int VertexArray::GetNumIndices() const
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Vertex array must be created before setting index buffer.");
        DEBUG_ASSERT(m_IndexBuffer != nullptr, "Vertex array must have an index buffer.");
        
        return m_IndexBuffer->GetCount();
    }
} // GoombaRender