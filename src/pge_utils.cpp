#include "pge_utils.h"

#include <fstream>
#include <utility>

namespace pge
{
    const bgfx::Memory *loadData(const char *_name)
    {
        const bgfx::Memory *mem;
        // char *data;
        std::ifstream file;
        size_t fileSize;
        file.open(_name);

        if (file.is_open())
        {
            file.seekg(0, std::ios::end);
            fileSize = file.tellg();

            mem = bgfx::alloc(fileSize + 1);

            file.seekg(0, std::ios::beg);
            file.read((char *)mem->data, fileSize);
            file.close();

            std::cout << "Readed: " << _name << " size: " << fileSize << '\n';
        }
        else
            std::cout << "Failed to read: " << _name << '\n';

        return mem;
    }

    bgfx::ShaderHandle loadShader(const char *_name)
    {
        const bgfx::Memory *mem = loadData(_name);
        mem->data[mem->size - 1] = '\0';
        bgfx::ShaderHandle handle = bgfx::createShader(mem);
        bgfx::setName(handle, _name);

        return handle;
    }

#ifndef PGE_TEXTURE_STORE_HEADER
#define PGE_TEXTURE_STORE_HEADER
    // texture storage
    std::unordered_map<std::string, _pge_texture_pt> PGE_TEXTURE_STORE;
#endif

    bgfx::TextureHandle loadTexture(const char *_name)
    {
        std::string texture_name = std::string(_name);

        if (PGE_TEXTURE_STORE.count(texture_name) > 0)
        {
            auto &texture = PGE_TEXTURE_STORE[texture_name];
            texture.pointers++;
            return texture.handle;
        }

        bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;
        const bgfx::Memory *mem = loadData(_name);

        bgfx::TextureInfo info;
        handle = bgfx::createTexture(mem, 0UL, 0U, &info);
        bgfx::setName(handle, _name);

        PGE_TEXTURE_STORE.insert(std::make_pair(texture_name, _pge_texture_pt{handle, 1}));

        return handle;
    }

    void unloadTexture(const char *_name)
    {
        std::string texture_name = std::string(_name);
        if (PGE_TEXTURE_STORE.count(texture_name) > 0)
        {
            auto &texture = PGE_TEXTURE_STORE[texture_name];
            texture.pointers--;

            if (texture.pointers == 0)
            {
                bgfx::destroy(texture.handle);
                PGE_TEXTURE_STORE.erase(texture_name);
            }
        }
        else
            std::cout << "WARNING failed attempt to erase an unexisting texture: " << _name << '\n';
    }

#ifndef PGE_NO_BGFX
    void mesh::initBuffers()
    {
        m_vbh = bgfx::createVertexBuffer(bgfx::makeRef(vertices, sizeof(vertices)), _pge_vertex_data::ms_layout);
        m_ibh = bgfx::createIndexBuffer(bgfx::makeRef(indices, sizeof(indices)));
    }

    mesh::~mesh()
    {
        bgfx::destroy(m_vbh);
        bgfx::destroy(m_ibh);

        delete vertices;
        delete indices;
    }
#else
    mesh::~mesh()
    {
        delete vertices;
        delete indices;
    }
#endif

    void mesh::load(std::ifstream &IN)
    {
        uint32_t size;
        // number of verticies
        IN >> size;
        vertices = new _pge_vertex_data[size];
        IN.read((char *)vertices, sizeof(vertices));

        //number of indices
        IN >> size;
        indices = new uint32_t[size];
        IN.read((char *)indices, sizeof(indices));
    }

    void mesh::save(std::ofstream &OUT)
    {
        uint32_t num_vertices = sizeof(vertices) / sizeof(_pge_vertex_data);
        OUT << num_vertices;
        OUT.write((char*)vertices, sizeof(vertices));

        uint32_t num_indices = sizeof(indices) / sizeof(uint32_t);
        OUT << num_indices;
        OUT.write((char*)num_indices, sizeof(num_indices));
    }

    void mesh::subdmit(bgfx::ProgramHandle &m_program)
    {
        bgfx::setVertexBuffer(0, m_vbh);
        bgfx::setIndexBuffer(m_ibh);
        bgfx::submit(0, m_program);
    }

    void model::subdmit(bgfx::ProgramHandle &m_program)
    {
        // Set model matrix for rendering.
        bgfx::setTransform(mtx);

        // subdmit all meshes
        for (auto &mh: meshes)
            mh.subdmit(m_program);
    }
}