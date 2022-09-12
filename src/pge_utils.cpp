#include "pge_utils.h"

#include <fstream>

namespace pge {
    const bgfx::Memory* loadData (const char *_name)
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
        } else 
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

    bgfx::TextureHandle loadTexture(const char *_name)
    {
        bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;
        const bgfx::Memory *mem = loadData(_name);

        bgfx::TextureInfo info;
        handle = bgfx::createTexture(mem, 0UL, 0U, &info);
        bgfx::setName(handle, _name);

        std::cout << "\ttexture resolution " << info.width << 'x' << info.height << '\n';

        return handle;
    }
}