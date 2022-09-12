#ifndef PGE_UTILS
#define PGE_UTILS

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
#include <stdexcept>

namespace pge {
    bgfx::ShaderHandle loadShader(const char *_name);
    bgfx::TextureHandle loadTexture(const char *_name);
}

#endif