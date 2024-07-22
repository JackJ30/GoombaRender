//
// Created by jack on 6/22/24.
//

#include "vertex_array.h"

#include <utility>

namespace GoombaRender
{
    VertexArray::VertexArray(LayoutType layoutType, GLenum drawMode)
            : layoutType(layoutType), drawMode(drawMode)
    {
        glad.GenVertexArrays(1, &rendererID);
    }
    
    VertexArray::VertexArray(unsigned int rendererID, LayoutType layoutType, GLenum drawMode)
            : rendererID(rendererID), layoutType(layoutType), drawMode(drawMode)
    {
    
    }
    
    void VertexArray::BindBufferLayout(unsigned int buffer, const BufferLayout &layout, size_t startingAttribute)
    {
        glad.BindVertexArray(rendererID);
        glad.BindBuffer(GL_ARRAY_BUFFER, buffer);
        
        size_t index = startingAttribute;
        for (const BufferElement& element : layout)
        {
            if (std::find(usedAttributes.begin(), usedAttributes.end(), index) != usedAttributes.end()) { GLogError("Vertex attribute index '{}' has already been set for VAO.", index); }
            usedAttributes.push_back(index);
            
            glad.EnableVertexAttribArray(index);
            glad.VertexAttribPointer(index, element.GetComponentCount(), element.GetGLType(), element.Normalized, layout.GetStride(), (const void*)element.Offset);
            index++;
        }
    }
    
    void VertexArray::BindAttribute(unsigned int buffer, size_t attributeIndex, unsigned int componentCount, GLenum glType, bool normalized, size_t stride, size_t offset)
    {
        if (std::find(usedAttributes.begin(), usedAttributes.end(), attributeIndex) != usedAttributes.end()) { GLogError("Vertex attribute '{}' has already been set for VAO.", attributeIndex); }
        usedAttributes.push_back(attributeIndex);
        
        glad.BindVertexArray(rendererID);
        glad.BindBuffer(GL_ARRAY_BUFFER, buffer);
        
        glad.EnableVertexAttribArray(attributeIndex);
        glad.VertexAttribPointer(attributeIndex, componentCount, glType, normalized, stride, (const void*)offset);
    }
    
    void VertexArray::SetIndexBuffer(unsigned int buffer, std::vector<IndicesSection> indicesInfo)
    {
        glad.BindVertexArray(rendererID);
        glad.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
        
        this->indicesInfo = std::move(indicesInfo);
    }
} // GoombaRender