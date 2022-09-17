// #include <assimp
#include <iostream>
#include <fstream>
#include <cstring>

#define PGE_NO_BGFX
#include "pge_utils.h"

int main(int argc, char **argv)
{
    std::ifstream IN;
    std::ofstream OUT;

    IN.open(argv[1], std::ios_base::in);
    OUT.open(argv[2], std::ios_base::binary | std::ios_base::out);


    return 0;
}