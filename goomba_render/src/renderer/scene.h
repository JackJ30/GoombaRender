#ifndef GOOMBARENDER_SCENE_H
#define GOOMBARENDER_SCENE_H

#include <utility>

#include "renderer/camera.h"
#include "model.h"
#include "shader.h"
#include "transform.h"

namespace GoombaRender
{
    struct SceneObject
    {
        SceneObject(std::shared_ptr<Model> model, Transform transform)
            : model(std::move(model)), transform(transform)
        {
        
        }
        
        std::shared_ptr<Model> model;
        Transform transform;
    };
    
    struct Scene
    {
        std::vector<SceneObject> m_Objects; // Temp
    };
}

// Scene responsibilities
// - Contain all data for a scene
// - Know what assets for loader to load
// - Know what objects for renderer to render
// - No camera (yet)
// - There will eventually be a SceneObject instead of only using models

#endif //GOOMBARENDER_SCENE_H
