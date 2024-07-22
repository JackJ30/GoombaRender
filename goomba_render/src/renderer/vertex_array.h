#ifndef GOOMBARENDER_VERTEX_ARRAY_H
#define GOOMBARENDER_VERTEX_ARRAY_H

#include <cstddef>
#include "renderer/graphics_context.h"
#include "renderer/buffer_layout.h"

// TODO - support dynamic and stream draw

// VAO abstraction GOAL
// - Store information about the buffer, how it is drawn and used
// - Make setting attributes easier (also keep track of how it is set

namespace GoombaRender
{
    enum LayoutType
    {
        Arrays,
        Indices
    };
    
    struct IndicesSection
    {
        size_t offset;
        unsigned int count;
        GLenum type;
    };
    
    struct VertexArray
    {
        VertexArray(LayoutType layoutType = LayoutType::Indices, GLenum drawMode = GL_TRIANGLES);
        VertexArray(unsigned int rendererID, LayoutType layoutType = LayoutType::Indices, GLenum drawMode = GL_TRIANGLES);
        
        unsigned int rendererID;
        LayoutType layoutType;
        GLenum drawMode;
        
        int numVertices; // must be set if not using an index buffer
        std::vector<IndicesSection> indicesInfo;
        
        std::vector<size_t> usedAttributes;
        
        void BindBufferLayout(unsigned int buffer, const BufferLayout& layout, size_t startingAttribute = 0);
        void BindAttribute(unsigned int buffer, size_t attributeIndex, unsigned int componentCount, GLenum glType, bool normalized, size_t stride, size_t offset);
        void SetIndexBuffer(unsigned int buffer, std::vector<IndicesSection> indicesInfo);
    };
    
} // GoombaRender

#endif //GOOMBARENDER_VERTEX_ARRAY_H
