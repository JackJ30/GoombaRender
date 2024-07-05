//
// Created by jack on 6/15/24.
//

#include "graphics_context.h"

void GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * msg, const void * data);

namespace GoombaEngine {
    void GraphicsContext::CreateContext(GLADloadfunc func) {
        GLogInfo("Loading OpenGL context...");
        if (!gladLoadGLContext(&m_GladContext, func))
        {
            GLogCritical("failed to load OpenGL function ptrs");
            return;
        }
        GLogTrace("OpenGL context loaded.");

        #ifdef DEBUG
        m_GladContext.Enable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        m_GladContext.DebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
        m_GladContext.DebugMessageCallback(GLDebugMessageCallback, NULL);
        #endif
    }
} // GoombaEngine

void GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * msg, const void * data)
{
    const char* _source;
    const char* _type;
    const char* _severity;
    
    switch (source) {
        case GL_DEBUG_SOURCE_API:
            _source = "API";
            break;
        
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            _source = "WINDOW SYSTEM";
            break;
        
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            _source = "SHADER COMPILER";
            break;
        
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            _source = "THIRD PARTY";
            break;
        
        case GL_DEBUG_SOURCE_APPLICATION:
            _source = "APPLICATION";
            break;
        
        case GL_DEBUG_SOURCE_OTHER:
            _source = "UNKNOWN";
            break;
        
        default:
            _source = "UNKNOWN";
            break;
    }
    
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            _type = "ERROR";
            break;
        
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            _type = "DEPRECATED BEHAVIOR";
            break;
        
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            _type = "UDEFINED BEHAVIOR";
            break;
        
        case GL_DEBUG_TYPE_PORTABILITY:
            _type = "PORTABILITY";
            break;
        
        case GL_DEBUG_TYPE_PERFORMANCE:
            _type = "PERFORMANCE";
            break;
        
        case GL_DEBUG_TYPE_OTHER:
            _type = "OTHER";
            break;
        
        case GL_DEBUG_TYPE_MARKER:
            _type = "MARKER";
            break;
        
        default:
            _type = "UNKNOWN";
            break;
    }
    
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            _severity = "HIGH";
            break;
        
        case GL_DEBUG_SEVERITY_MEDIUM:
            _severity = "MEDIUM";
            break;
        
        case GL_DEBUG_SEVERITY_LOW:
            _severity = "LOW";
            break;
        
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            _severity = "NOTIFICATION";
            break;
        
        default:
            _severity = "UNKNOWN";
            break;
    }
    
    GLogInfo("|||OPENGL ISSUE||| ID: (%d), Type: (%s), Severity : (%s), raised from %s --> %s\n",
           id, _type, _severity, _source, msg);
}