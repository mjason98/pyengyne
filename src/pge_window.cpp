#include "pge_window.h"
#include "pge_data.h"

namespace pge
{
    PgeWindow::PgeWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name}
    {
        initWindow();
    }

    PgeWindow::~PgeWindow()
    {
        // todo destroy programs, modles, textures
        
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

    bgfx::VertexLayout PosColorVertex::ms_layout;

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

        init.type = bgfx::RendererType::OpenGL; // todo: put flags for vulkan
        init.resolution.width = (uint32_t)width;
        init.resolution.height = (uint32_t)height;
        init.resolution.reset = BGFX_RESET_VSYNC;
        bgfx::init(init);

        // Set view 0 to the same dimensions as the window and to clear the color buffer.
        bgfx::setViewClear(0
			, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
			, 0x303030ff
			, 1.0f
			, 0
			);


        // bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);

        // -------------------------

        PosColorVertex::init();

        m_vbh = bgfx::createVertexBuffer(
            // Static data can be passed with bgfx::makeRef
            bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)),
            PosColorVertex::ms_layout);

        m_ibh = bgfx::createIndexBuffer(
            // Static data can be passed with bgfx::makeRef
            bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));


        vsh = loadShader(SHADER_SIMPLE_VS);
        fsh = loadShader(SHADER_SIMPLE_FS);

        m_program = bgfx::createProgram(vsh, fsh, true);

        texture1 = loadTexture(TEXTURE_TEST);
    }

    // ------------------------------------------------------------------

    // ------------------------------------------------------------------

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
        // bgfx::touch(kClearView);

#ifdef DEBUG
        // Enable stats or debug text.
        bgfx::setDebug(s_showStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT);
#endif
        // Advance to next frame. Process submitted rendering primitives.
        // bgfx::frame();

        const bx::Vec3 at = {0.0f, 0.0f, 0.0f};
        const bx::Vec3 eye = {0.0f, 0.0f, 10.0f};

        // Set view and projection matrix for view 0.
        float view[16];
        bx::mtxLookAt(view, eye, at);

        float proj[16];
        bx::mtxProj(proj, 60.0f, float(width) / float(height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);

        bgfx::setViewTransform(0, view, proj);

        // Set view 0 default viewport.
        bgfx::setViewRect(0, 0, 0, width, height);

        bgfx::touch(0);

        float mtx[16];
        bx::mtxRotateY(mtx, 0.0f);

        // position x,y,z
        mtx[12] = 0.0f;
        mtx[13] = 0.0f;
        mtx[14] = 0.0f;

        // Set model matrix for rendering.
        bgfx::setTransform(mtx);

        // Set vertex and index buffer.
        bgfx::setVertexBuffer(0, m_vbh);
        bgfx::setIndexBuffer(m_ibh);

        // Set render states.
        bgfx::setState(BGFX_STATE_DEFAULT);

        // Submit primitive for rendering to view 0.
        bgfx::submit(0, m_program);

        bgfx::frame();
    }
}