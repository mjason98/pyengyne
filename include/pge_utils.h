#ifndef PGE_UTILS
#define PGE_UTILS

#ifdef DEBUG
#define BX_CONFIG_DEBUG 1
#elif
#define BX_CONFIG_DEBUG 0
#endif

#define BGFX_CONFIG_RENDERER_OPENGL 44

#ifndef PGE_NO_BGFX // used in tools

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
#include <unordered_map>
#include <vector>
#include "pge_data.h"

#endif // used in tools

namespace pge
{
#ifndef PGE_NO_BGFX // used in tools
    // Store texture type
    struct _pge_texture_pt
    {
        bgfx::TextureHandle handle; // bgfx handle for textures
        uint32_t pointers;          // how many object load the texture, if equals 0, the texture is removed
    };

    bgfx::ShaderHandle loadShader(const char *_name);
    bgfx::TextureHandle loadTexture(const char *_name);
    void unloadTexture(const char *_name);
#endif // used in tools

    // mesh and models
    struct _pge_vertex_data
    {
        // position
        float m_x;
        float m_y;
        float m_z;
        // normal
        uint32_t m_normal;
        // texture cordinates
        float m_u;
        float m_v;

#ifndef PGE_NO_BGFX // used in tools
        static void init()
        {
            ms_layout
                .begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Normal, 4, bgfx::AttribType::Uint8, true, true)
                .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
                .end();
        };
        static bgfx::VertexLayout ms_layout;
#endif // used in tools
    };

    class mesh
    {
    private:
        _pge_vertex_data *vertices;
        uint32_t *indices;
#ifndef PGE_NO_BGFX
        // bgfx::TextureHandle m_diffuse, m_normal;
        bgfx::VertexBufferHandle m_vbh;
        bgfx::IndexBufferHandle m_ibh;

    public:
        void initBuffers();
        void subdmit(bgfx::ProgramHandle &m_program);
#endif
    public:
        ~mesh();
        void load(std::ifstream &IN);
        void save(std::ofstream &OUT);
    };

    class model
    {
        private:
        std::vector<mesh> meshes;
        
        // model matriz
        float mtx[16];
        game_object_info goi;

        void subdmit(bgfx::ProgramHandle &m_program);
    };

}

#endif