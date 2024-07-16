#ifndef GOOMBARENDER_MODEL_H
#define GOOMBARENDER_MODEL_H

#include "renderer/vertex_array.h"

namespace GoombaRender
{
    struct Mesh
    {
        unsigned int vao;
        std::vector<unsigned int> textures;
    };
    
    class Model : OglObj
    {
    public:
        void Create();
        void Delete();
        
    private:
        std::vector<Mesh> m_Meshes;
    };
    
} // GoombaRender

#endif //GOOMBARENDER_MODEL_H
