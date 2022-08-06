#include "pge_app.h"

namespace pge {
    void App::run()
    {
        while (!pgeWindow.shouldClose())
        {
            glfwPollEvents();
            pgeWindow.updateRenderer();
        }
        
    }
}