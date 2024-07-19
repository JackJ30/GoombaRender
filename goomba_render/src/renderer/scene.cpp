#include "scene.h"

namespace GoombaRender
{
    void Scene::LoadAssets(GoombaEngine::GraphicsContext &context)
    {
        for (SceneObject& object : m_Objects)
        {
            LoadModel(object.model, context);
        }
        LoadShader(m_MainShader, context);
    }
}