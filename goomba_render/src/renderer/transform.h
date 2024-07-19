#ifndef GOOMBARENDER_TRANSFORM_H
#define GOOMBARENDER_TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace GoombaRender
{
    class Transform
    {
    public:
        Transform();
        explicit Transform(glm::vec3 translation);
        
        inline glm::vec3 GetTranslation() const { return m_Translation; }
        inline void SetTranslation(glm::vec3 translation) { m_Translation = translation; CalculateMatrix(); }
        inline glm::quat GetRotation() const { return m_Rotation; }
        inline void SetRotation(glm::quat rotation) { m_Rotation = rotation; CalculateMatrix(); }
        inline glm::vec3 GetScale() const { return m_Scale; }
        inline void SetScale(glm::vec3 scale) { m_Scale = scale; CalculateMatrix(); }
        
        inline glm::mat4 GetTransformationMatrix() const { return m_Matrix; }
        inline void SetTransformationMatrix(glm::mat4 matrix) { m_Matrix = matrix; DecomposeMatrix(); }
        
    private:
        glm::vec3 m_Translation;
        glm::quat m_Rotation;
        glm::vec3 m_Scale;
        
        glm::mat4 m_Matrix;
        
        void CalculateMatrix();
        void DecomposeMatrix();
    };
    
} // GoombaRender

#endif //GOOMBARENDER_TRANSFORM_H
