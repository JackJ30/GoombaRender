//
// Created by jack on 6/22/24.
//

#include "vertex_array.h"

namespace GoombaRender
{
    VertexArray::VertexArray(GoombaEngine::GraphicsContext context)
        : m_Context(context)
    {
        m_Context.GetGlad().GenVertexArrays(1, &m_RendererID);
    }
    
    void VertexArray::Bind() const
    {
        m_Context.GetGlad().BindVertexArray(m_RendererID);
    }
    
    void VertexArray::Unbind() const
    {
        m_Context.GetGlad().BindVertexArray(0);
    }
    
    void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer)
    {
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
        m_Context.GetGlad().BindVertexArray(m_RendererID);
        indexBuffer->Bind();
        
        m_IndexBuffer = indexBuffer;
    }
} // GoombaRender