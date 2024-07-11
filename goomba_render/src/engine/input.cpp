#include "input.h"

namespace GoombaEngine
{
    void Input::ProcessEvent(SDL_Event &event)
    {
        switch(event.type)
        {
            case SDL_EVENT_KEY_UP:
            {
                m_PressedKeys.erase(event.key.keysym.sym);
                break;
            }
            case SDL_EVENT_KEY_DOWN:
            {
                m_PressedKeys.insert(event.key.keysym.sym);
                break;
            }
        }
    }
    
    bool Input::IsKeyPressed(SDL_Keycode keycode)
    {
        return m_PressedKeys.count(keycode) != 0;
    }
    
    bool Input::IsKeyJustPressed(SDL_Keycode keycode)
    {
        return false; // TODO - implement
    }
    
    bool Input::IsKeyJustReleased(SDL_Keycode keycode)
    {
        return false; // TODO - implement
    }
} // GoombaEngine