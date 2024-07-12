#ifndef GOOMBARENDER_PERSPECTIVE_CAMERA_H
#define GOOMBARENDER_PERSPECTIVE_CAMERA_H

#include "renderer/camera.h"

namespace GoombaRender
{
    class PerspectiveCamera : public Camera
    {
    public:
        PerspectiveCamera(glm::vec3 position = {0.0f, 0.0f, 0.0f}, float fov = glm::radians(60.0), float aspect = 16/9, float near = 0.1, float far = 100, float pitch = 0.0f, float yaw = -90.0f, glm::vec3 worldUp = {0.0f, 1.0f, 0.0f});
        glm::mat4 GetProjectionMatrix() const override;
        void SetFov(float fov);
        void SetClipping(float near, float far);
    private:
        float m_Fov, m_Near, m_Far;
    };
    
} // GoombaRender

#endif //GOOMBARENDER_PERSPECTIVE_CAMERA_H

