#include "pge_utils.h"

#include <fstream>

namespace pge {
    char* loadData (const char *_name, size_t* _fileSize)
    {
        char *data;
        std::ifstream file;
        size_t fileSize;
        file.open(_name);
        
        if (file.is_open())
        {
            file.seekg(0, std::ios::end);
            fileSize = file.tellg();

            data = new char[fileSize + 1];

            file.seekg(0, std::ios::beg);
            file.read(data, fileSize);
            file.close();

            std::cout << "Readed: " << _name << " size: " << fileSize << '\n';
        } else 
            std::cout << "Failed to read: " << _name << '\n';
        
        *_fileSize = fileSize;
        return data;
    }

    bgfx::ShaderHandle loadShader(const char *_name)
    {
        size_t fileSize;
        char *data = loadData(_name, &fileSize);

        const bgfx::Memory *mem = bgfx::copy(data, fileSize + 1);
        mem->data[mem->size - 1] = '\0';
        bgfx::ShaderHandle handle = bgfx::createShader(mem);
        bgfx::setName(handle, _name);

        delete data;

        return handle;
    }

    bgfx::TextureHandle loadTexture(const char *_name)
    {
        bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;

        // todo use alloc
        size_t fileSize;
        char *data = loadData(_name, &fileSize);

        const bgfx::Memory *mem = bgfx::copy(data, fileSize + 1);
        mem->data[mem->size - 1] = '\0';

        bgfx::TextureInfo info;

        handle = bgfx::createTexture(mem, 0UL, 0U, &info);

        std::cout << "texture size " << info.width << 'x' << info.height << '\n';

        // size_t fileSize;
        // char *data = loadData(_name, &fileSize);

        // const bgfx::Memory *mem = bgfx::copy(data, fileSize + 1);
        // mem->data[mem->size - 1] = '\0';
        // bgfx::ShaderHandle handle = bgfx::createShader(mem);
        // bgfx::setName(handle, _name);

        // delete data;

        return handle;
    }
}