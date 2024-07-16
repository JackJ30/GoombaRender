#ifndef GOOMBARENDER_SCENE_H
#define GOOMBARENDER_SCENE_H

#include "renderer/asset.h"
#include "renderer/camera.h"

namespace GoombaRender
{
    struct Scene
    {
        std::vector<std::string> m_ModelsToLoad;
        std::vector<ModelAsset> m_Objects;
    };
}

// Scene responsibilities
// - Contain all data for a scene
// - Know what assets for loader to load
// - Know what objects for renderer to render
// - No camera (yet)
// - There will eventually be a SceneObject instead of only using models

#endif //GOOMBARENDER_SCENE_H
