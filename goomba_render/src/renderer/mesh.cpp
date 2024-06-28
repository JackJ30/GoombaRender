#include "mesh.h"

#include <glad/gl.h>

#define ogl context.GetGladContext()

namespace GoombaRender
{
    Mesh::Mesh(GoombaEngine::GraphicsContext context, std::vector<float> vertexArray, unsigned int dimensions) {
        ogl.GenVertexArrays(1, &m_VAO);
        ogl.BindVertexArray(m_VAO);
        
        ogl.GenBuffers(1, &m_PositionVBO);
        ogl.BindBuffer(GL_ARRAY_BUFFER, m_PositionVBO);
        ogl.BufferData(m_PositionVBO, vertexArray.size() * sizeof(float), &vertexArray[0], GL_STATIC_DRAW);
    }

    Mesh::~Mesh() {

    }
}