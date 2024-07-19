#include "transform.h"

#include <glm/gtx/matrix_decompose.hpp>

namespace GoombaRender
{
    Transform::Transform()
    {
        SetTransformationMatrix(glm::mat4(1.0));
    }
    
    Transform::Transform(glm::vec3 translation)
    {
        SetTransformationMatrix(glm::mat4(1.0));
        SetTranslation(translation);
    }
    
    void Transform::CalculateMatrix()
    {
        m_Matrix = glm::translate(glm::scale(glm::mat4_cast(m_Rotation), m_Scale), m_Translation);
    }
    
    void Transform::CalculateComponents()
    {
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(m_Matrix, m_Scale, m_Rotation, m_Translation, skew, perspective);
    }
} // GoombaRender