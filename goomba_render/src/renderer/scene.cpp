#include "scene.h"

namespace GoombaRender
{
    void Scene::LoadAssets(GoombaEngine::GraphicsContext &context)
    {
        for (Asset<Model>& model : m_Objects)
        {
            LoadModel(model, context);
        }
        LoadShader(m_MainShader, context);
    }
}