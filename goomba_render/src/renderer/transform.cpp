#include "transform.h"

#include <glm/gtx/matrix_decompose.hpp>

namespace GoombaRender
{
    void Transform::CalculateMatrix()
    {
        m_Matrix = glm::translate(glm::scale(glm::mat4_cast(m_Rotation), m_Scale), m_Translation);
    }
    
    void Transform::DecomposeMatrix()
    {
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(m_Matrix, m_Scale, m_Rotation, m_Translation, skew, perspective);
    }
} // GoombaRender