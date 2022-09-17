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
}