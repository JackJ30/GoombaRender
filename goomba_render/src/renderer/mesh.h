#ifndef GOOMBARENDER_MESH_H
#define GOOMBARENDER_MESH_H

#include "engine/graphics_context.h"

namespace GoombaRender
{
    class Mesh {
    public:
        Mesh(GoombaEngine::GraphicsContext context, std::vector<float> vertexArray, unsigned int dimensions);
        ~Mesh();

    private:
        unsigned int m_VAO;
        unsigned int m_PositionVBO;
    };
}

#endif //GOOMBARENDER_MESH_H
