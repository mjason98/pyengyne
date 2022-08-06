#pragma once

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

namespace pge
{
    class PgeWindow
    {
    public:
        PgeWindow(int w, int h, std::string name);
        ~PgeWindow();

        PgeWindow(const PgeWindow &) = delete;
        PgeWindow &operator=(const PgeWindow &) = delete; 

        bool shouldClose();

    private:
        void initWindow();

        const int width;
        const int height;

        std::string windowName;
        GLFWwindow *window;
    };
}