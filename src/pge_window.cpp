#include "pge_window.h"

#include <stdexcept>

namespace pge
{
    PgeWindow::PgeWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name}
    {
        initWindow();
    }

    PgeWindow::~PgeWindow()
    {
        bgfx::shutdown();
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    bool PgeWindow::shouldClose()
    {
        return glfwWindowShouldClose(window);
    }

    static void glfw_errorCallback(int error, const char *description)
    {
        fprintf(stderr, "GLFW error %d: %s\n", error, description);
    }

    static void glfw_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
#ifdef DEBUG
        if (key == GLFW_KEY_F1 && action == GLFW_RELEASE)
            s_showStats = !s_showStats;
#endif
    }

    void PgeWindow::initWindow()
    {
        glfwSetErrorCallback(glfw_errorCallback);

        if (!glfwInit())
            throw std::runtime_error("failed to init glfw");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        if (!window)
            throw std::runtime_error("failed to create a glfw's window");

        glfwSetKeyCallback(window, glfw_keyCallback);
        // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
        // Most graphics APIs must be used on the same thread that created the window.
        bgfx::renderFrame();
        // Initialize bgfx using the native window handle and window resolution.

        bgfx::Init init;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
        init.platformData.ndt = glfwGetX11Display();
        init.platformData.nwh = (void *)(uintptr_t)glfwGetX11Window(window);
#elif BX_PLATFORM_OSX
        init.platformData.nwh = glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
        init.platformData.nwh = glfwGetWin32Window(window);
#endif

        init.type = bgfx::RendererType::OpenGL;
        init.resolution.width = (uint32_t)width;
        init.resolution.height = (uint32_t)height;
        init.resolution.reset = BGFX_RESET_VSYNC;
        bgfx::init(init);

        std::cout << "here\n";

        // Set view 0 to the same dimensions as the window and to clear the color buffer.
        bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR);
        bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);

        std::cout << "here\n";
    }

    void PgeWindow::updateRenderer()
    {
        // Handle window resize.
        int oldWidth = width, oldHeight = height;
        glfwGetWindowSize(window, &width, &height);
        if (width != oldWidth || height != oldHeight)
        {
            bgfx::reset((uint32_t)width, (uint32_t)height, BGFX_RESET_VSYNC);
            bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
        }
        // This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
        bgfx::touch(kClearView);
        // Use debug font to print information about this example.
        bgfx::dbgTextClear();
        bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
        bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
        bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
        bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");
        const bgfx::Stats *stats = bgfx::getStats();
        bgfx::dbgTextPrintf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.", stats->width, stats->height, stats->textWidth, stats->textHeight);
        // Enable stats or debug text.
        bgfx::setDebug(s_showStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT);
        // Advance to next frame. Process submitted rendering primitives.
        bgfx::frame();
    }
}