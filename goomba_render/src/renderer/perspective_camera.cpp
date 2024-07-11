#include "perspective_camera.h"

namespace GoombaRender
{
    PerspectiveCamera::PerspectiveCamera(glm::vec3 position, float fov, float aspect, float near, float far, float pitch,
                                         float yaw, glm::vec3 worldUp)
                                         : Camera(position, pitch, yaw, worldUp), m_Aspect(aspect), m_Near(near), m_Far(far), m_Fov(fov)
    {
    
    }
    
    glm::mat4 PerspectiveCamera::GetProjectionMatrix() const
    {
        return glm::perspective(m_Fov, m_Aspect, m_Near, m_Far);
    }
    
    void PerspectiveCamera::SetFov(float fov)
    {
        m_Fov = fov;
    }
    
    void PerspectiveCamera::SetAspect(float aspect)
    {
        m_Aspect = aspect;
    }
    
    void PerspectiveCamera::SetClipping(float near, float far)
    {
        m_Near = near;
        m_Far = far;
    }
} // GoombaRender