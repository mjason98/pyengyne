#pragma once

#include "pge_window.h"

#include <memory>
#include <vector>

namespace pge
{
    class App
    {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
        
        App(){}
        ~App(){}

        App(const App&) = delete;
        App &operator=(const App &) = delete;

        void run();
    private:
        PgeWindow pgeWindow {WIDTH, HEIGHT, "Hello Bgfx!"};
    };
}