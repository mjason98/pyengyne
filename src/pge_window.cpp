#include "pge_window.h"

#include <stdexcept>

namespace pge {
    PgeWindow::PgeWindow(int w, int h, std::string name) :
     width{w}, height{h}, windowName{name} {
        initWindow();
     }

    PgeWindow::~PgeWindow()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    bool PgeWindow::shouldClose()
    {
        return glfwWindowShouldClose(window); 
    }
    
    void PgeWindow::initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    }
}