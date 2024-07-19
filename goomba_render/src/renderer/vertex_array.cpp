//
// Created by jack on 6/22/24.
//

#include "vertex_array.h"

#include <utility>

namespace GoombaRender
{
    void VertexArray::Create(DrawType drawType, GLenum drawMode)
    {
        RequireContext();
        
        m_DrawType = drawType;
        m_DrawMode = drawMode;
        m_Context.GetGlad().GenVertexArrays(1, &m_RendererID);
        m_Created = true;
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
    
    void VertexArray::Delete()
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Vertex array must be created before deleting.");
        
        m_Context.GetGlad().DeleteVertexArrays(1, &m_RendererID);
        
        for (unsigned int buffer : m_OwnedBuffers)
        {
            m_Context.GetGlad().DeleteBuffers(1, &buffer);
        }
    }
    
    void VertexArray::CreateVertexBuffer(float *vertices, size_t numVertices, const BufferLayout &layout)
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Vertex array must be created before creating vertex buffer.");
        
        m_NumVertices = numVertices;
        
        m_Context.GetGlad().GenVertexArrays(1, &m_RendererID);
        m_Context.GetGlad().BindVertexArray(m_RendererID);
        
        unsigned int vbo;
        m_Context.GetGlad().GenBuffers(1, &vbo);
        m_Context.GetGlad().BindBuffer(GL_ARRAY_BUFFER, vbo);
        m_Context.GetGlad().BufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float), vertices, GL_STATIC_DRAW);
        m_OwnedBuffers.insert(vbo);
        
        size_t index = 0;
        for (const BufferElement& element : layout)
        {
            if (std::find(m_UsedAttributes.begin(), m_UsedAttributes.end(),index) != m_UsedAttributes.end()) { GLogError("Vertex attribute index '{}' has already been set for VAO.", index); }
            m_UsedAttributes.push_back(index);
            
            m_Context.GetGlad().EnableVertexAttribArray(index);
            m_Context.GetGlad().VertexAttribPointer(index, element.GetComponentCount(), element.GetGLType(), element.Normalized, layout.GetStride(), (const void*)element.Offset);
            index++;
        }
    }
    
    void VertexArray::CreateIndexBuffer(unsigned int *indices, unsigned int numIndices)
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Vertex array must be created before creating index buffer.");
        
        m_Context.GetGlad().BindVertexArray(m_RendererID);
        
        unsigned int ibo;
        m_Context.GetGlad().GenBuffers(1, &ibo);
        m_Context.GetGlad().BindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        m_Context.GetGlad().BufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
        m_OwnedBuffers.insert(ibo);
        
        m_IndicesInfo.push_back({0, numIndices});
    }
    
    void VertexArray::BindAttribute(unsigned int buffer, size_t attributeIndex, unsigned int componentCount,
                                    GLenum glType, bool normalized, size_t stride, size_t offset)
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Vertex array must be created before adding single attribute.");
        
        if (std::find(m_UsedAttributes.begin(), m_UsedAttributes.end(),attributeIndex) != m_UsedAttributes.end()) { GLogError("Vertex attribute '{}' has already been set for VAO.", attributeIndex); }
        m_UsedAttributes.push_back(attributeIndex);
        
        m_Context.GetGlad().BindVertexArray(m_RendererID);
        m_Context.GetGlad().BindBuffer(GL_ARRAY_BUFFER, buffer);
        
        m_Context.GetGlad().EnableVertexAttribArray(attributeIndex);
        m_Context.GetGlad().VertexAttribPointer(attributeIndex, componentCount, glType, normalized, stride, (const void*)offset);
    }
    
    void VertexArray::SetIndexBuffer(unsigned int buffer, std::vector<IndicesSection> indicesInfo)
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Vertex array must be created before setting index buffer.");
        
        m_Context.GetGlad().BindVertexArray(m_RendererID);
        m_Context.GetGlad().BindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
        
        m_IndicesInfo = std::move(indicesInfo);
    }
} // GoombaRender