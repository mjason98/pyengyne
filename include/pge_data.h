#ifndef PGE_DATA
#define PGE_DATA

namespace pge {
    /// shaders 
    #define SHADER_SIMPLE_VS "bin/shaders/vs_simple.bin"
    #define SHADER_SIMPLE_FS "bin/shaders/fs_simple.bin"
    /// textures
    #define TEXTURE_TEST "bin/data/mushroom.dds"

    struct game_object_info
    {
        // position
        float x, y, z;
        // scale
        float sx, xy, sz;
        // rotation
        // todo
    };
}

#endif