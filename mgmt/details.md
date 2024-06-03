# Features
## Goomba Engine
- Resource Management: Importing, Loading, Packing
- Window System
- Event System
- Application + Layers System
- Input System
- Render/Tick Loop
- API Management (Long Shot)
- ECS (Long Shot)

## Goomba Render
- Basic Render API Abstractions
- Render Pipeline
- Draw call management
- Scene system
    - Model system
    - Light system
    - Data based
- Deferred Rendering
- Clustered Forward Rendering
- Shadow Mapping
- Blending
- Additional Rendering Techniques

# The Plan
## Phase 1:
- Setup
- Basic application, window
- No real event or input system
- Hardcoded ImGUI
- OpenGL abstractions
- Hardcoded render pipeline
- Model loading
- Material system
- Code based scene system
- Forward rendering lighting
- Basic render loop

## Phase 2:
- Event and input system
- Layer system
- Deferred rendering
- Shadow mapping
- Blending
- Clearing and framebuffer management
- Draw call management

## Phase 3:
- Resource importing
- Data based scenes
- Tick loop
- PBR
- Clustered forward rendering

## Phase ?:
- More rendering techniques
- Data based rendering pipeline?