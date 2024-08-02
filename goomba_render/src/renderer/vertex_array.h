#ifndef GOOMBARENDER_VERTEX_ARRAY_H
#define GOOMBARENDER_VERTEX_ARRAY_H

#include <cstddef>

#include "renderer/graphics_context.h"
#include "renderer/buffer_layout.h"

// TODO - support dynamic and stream draw

// VA abstraction GOAL
// - Store information about the buffer, how it is drawn and used
// - Make setting attributes easier (also keep track of how it is set
// - Metadata and methods for VAO

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
    
    struct VertexArrayInfo
    {
    public:
        explicit VertexArrayInfo(unsigned int rendererID, LayoutType layoutType = LayoutType::Indices, GLenum drawMode = GL_TRIANGLES);
        
        VertexArrayInfo(const VertexArrayInfo&) = delete;
        VertexArrayInfo& operator=(const VertexArrayInfo&) = delete;
        
        GLenum drawMode;
        std::vector<size_t> usedAttributes;
        
        // select layoutType, and then pick numVertices or IndicesInfo.
        // TODO - make "layout type/strategy" object with indexed and arrays
        LayoutType layoutType;
        int numVertices;
        std::vector<IndicesSection> indicesInfo;
    private:
        unsigned int rendererID;
        
    public:
        void BindBufferLayout(unsigned int buffer, const BufferLayout& layout, size_t startingAttribute = 0);
        void BindAttribute(unsigned int buffer, size_t attributeIndex, unsigned int componentCount, GLenum glType, bool normalized, size_t stride, size_t offset);
        void SetIndexBuffer(unsigned int buffer, std::vector<IndicesSection> indicesInfo);
        
        inline const unsigned int GetRendererID() const { return rendererID; }
        
        inline void Delete() { glad.DeleteVertexArrays(1, &rendererID); }
    };
    
    // Creation functions
    VertexArrayInfo* CreateVertexArray(LayoutType layoutType = LayoutType::Indices, GLenum drawMode = GL_TRIANGLES);
    
} // GoombaRender

#endif //GOOMBARENDER_VERTEX_ARRAY_H
