//
// Created by jack on 6/22/24.
//

#ifndef GOOMBARENDER_VERTEX_ARRAY_H
#define GOOMBARENDER_VERTEX_ARRAY_H

#include "renderer/ogl_obj.h"
#include "renderer/buffer_layout.h"

// TODO - support dynamic and stream draw

namespace GoombaRender
{
    enum DrawType
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
    
    class VertexArray : public OglObj
    {
    public:
        VertexArray() = default;
        ~VertexArray() = default;
        
        void Create(DrawType drawType, GLenum drawMode = GL_TRIANGLES);
        void Delete();
        
        void Bind() const;
        void Unbind() const;
        
        // Creation Approach
        void CreateVertexBuffer(float* vertices, size_t numVertices, const BufferLayout &layout);
        void CreateIndexBuffer(unsigned int *indices, unsigned int numIndices);
        
        // Manual Approach ()
        void BindAttribute(unsigned int buffer, size_t attributeIndex, unsigned int componentCount, GLenum glType, bool normalized, size_t stride, size_t offset);
        void SetIndexBuffer(unsigned int buffer, std::vector<IndicesSection> indicesInfo);
        
        inline DrawType GetDrawType() const { return m_DrawType; };
        inline GLenum GetDrawMode() const { return m_DrawMode; };
        
        inline const std::vector<IndicesSection>& GetIndicesSections() const { return m_IndicesInfo; }
        
        // If manual approach is used without setting index buffer, num vertices must be set.
        inline void SetNumVertices(unsigned int numVertices) { m_NumVertices = numVertices; }
        inline unsigned int GetNumVertices() const { return m_NumVertices; }
    private:
        unsigned int m_RendererID;
        DrawType m_DrawType;
        GLenum m_DrawMode;
        
        unsigned int m_NumVertices = 0; // Arrays
        std::vector<IndicesSection> m_IndicesInfo; // Indices
        
        std::vector<size_t> m_UsedAttributes;
        std::unordered_set<unsigned int> m_OwnedBuffers;
    };
    
} // GoombaRender

#endif //GOOMBARENDER_VERTEX_ARRAY_H
