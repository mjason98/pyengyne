/*
 * Copyright 2011-2019 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#pragma once

#ifdef DEBUG
#define BX_CONFIG_DEBUG 1
#elif
#define BX_CONFIG_DEBUG 0
#endif

#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <string>
#include <iostream>

namespace pge
{
#ifdef DEBUG
    static bool s_showStats = true;
#endif

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
    };
}