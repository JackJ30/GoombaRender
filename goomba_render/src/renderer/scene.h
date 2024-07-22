#ifndef GOOMBARENDER_SCENE_H
#define GOOMBARENDER_SCENE_H

#include "renderer/asset.h"
#include "renderer/camera.h"
#include "model.h"
#include "shader.h"
#include "transform.h"

namespace GoombaRender
{
    struct SceneObject
    {
        Asset<Model> model;
        Transform transform;
    };
    
    class Scene
    {
    public:
        std::vector<SceneObject> m_Objects; // Temp
        Asset<Shader> m_MainShader = Asset<Shader>("resources/shaders/test.glsl"); // Temp, also should be material
        
        void LoadAssets();
    };
}

// Scene responsibilities
// - Contain all data for a scene
// - Know what assets for loader to load
// - Know what objects for renderer to render
// - No camera (yet)
// - There will eventually be a SceneObject instead of only using models

#endif //GOOMBARENDER_SCENE_H
