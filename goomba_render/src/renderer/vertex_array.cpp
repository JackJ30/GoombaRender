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
    
    void
    VertexArray::AddVertexBuffer(float *vertices, size_t size, const BufferLayout &layout, unsigned int startingAttributeIndex)
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Vertex array must be created before adding vertex buffer.");
        
        m_Context.GetGlad().BindVertexArray(m_RendererID);
        
        unsigned int vbo;
        m_Context.GetGlad().GenBuffers(1, &vbo);
        m_Context.GetGlad().BindBuffer(GL_ARRAY_BUFFER, vbo);
        m_Context.GetGlad().BufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        
        size_t index = startingAttributeIndex;
        for (const BufferElement& element : layout)
        {
            if (std::find(m_UsedAttributes.begin(), m_UsedAttributes.end(),index) != m_UsedAttributes.end()) { GLogError("Vertex attribute '{}' has already been set for VAO.", index); }
            m_UsedAttributes.push_back(index);
            
            m_Context.GetGlad().EnableVertexAttribArray(index);
            m_Context.GetGlad().VertexAttribPointer(index, element.GetComponentCount(), element.GetGLType(), element.Normalized, layout.GetStride(), (const void*)element.Offset);
            index++;
        }
        
        m_VertexBuffers.push_back(vbo);
    }
    
    void VertexArray::AddSingleAttribute(unsigned int vbo, size_t attributeIndex, unsigned int componentCount,
                                         GLenum glType, bool normalized, size_t stride, size_t offset)
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Vertex array must be created before adding single attribute.");
        
        if (std::find(m_UsedAttributes.begin(), m_UsedAttributes.end(),attributeIndex) != m_UsedAttributes.end()) { GLogError("Vertex attribute '{}' has already been set for VAO.", attributeIndex); }
        m_UsedAttributes.push_back(attributeIndex);
        m_VertexBuffers.push_back(vbo);
        
        m_Context.GetGlad().BindVertexArray(m_RendererID);
        m_Context.GetGlad().BindBuffer(GL_ARRAY_BUFFER, vbo);
        
        m_Context.GetGlad().EnableVertexAttribArray(attributeIndex);
        m_Context.GetGlad().VertexAttribPointer(attributeIndex, componentCount, glType, normalized, stride, (const void*)offset);
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