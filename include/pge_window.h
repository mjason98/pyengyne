#pragma once

#ifdef DEBUG
#define BX_CONFIG_DEBUG 1
#elif
#define BX_CONFIG_DEBUG 0
#endif

#define BGFX_CONFIG_RENDERER_OPENGL 44

#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/math.h>

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include <GLFW/glfw3native.h>
#include <string>
#include <iostream>

namespace pge
{
#ifdef DEBUG
    static bool s_showStats = false;
#endif
    
    bgfx::ShaderHandle loadShader(const char *_name);

    struct PosColorVertex
    {
        // 3d space position
        float m_x;
        float m_y;
        float m_z;
        // Color value
        uint32_t m_abgr;

        static void init()
        {
            // start the attribute declaration
            ms_layout
                .begin()
                // Has three float values that denote position
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                // and a uint8 color value that denotes color
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
                .end();
        };

        static bgfx::VertexLayout ms_layout;
    };

    // bgfx::VertexLayout PosColorVertex::ms_layout;


    static PosColorVertex s_cubeVertices[] =
{
    {  0.5f,  0.5f, 0.0f, 0xff0000ff },
    {  0.5f, -0.5f, 0.0f, 0xff0000ff },
    { -0.5f, -0.5f, 0.0f, 0xff00ff00 },
    { -0.5f,  0.5f, 0.0f, 0xff00ff00 }
};

static const uint16_t s_cubeTriList[] =
{
    0,1,3,
    1,2,3
};
    
    
    class PgeWindow
    {
    public:
        PgeWindow(int w, int h, std::string name);
        ~PgeWindow();

        PgeWindow(const PgeWindow &) = delete;
        PgeWindow &operator=(const PgeWindow &) = delete;

        bool shouldClose();
        void updateRenderer();

    private:
        void initWindow();

        int width, height;
        const bgfx::ViewId kClearView = 0;

        std::string windowName;
        GLFWwindow *window;

        bgfx::VertexBufferHandle m_vbh;
        bgfx::IndexBufferHandle m_ibh;

        bgfx::ShaderHandle vsh;
        bgfx::ShaderHandle fsh;
        bgfx::ProgramHandle m_program;
    };
}