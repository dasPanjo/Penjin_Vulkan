#include "ApplicationWindow.h"
#include <string>

using namespace Penjin;

ApplicationWindow::ApplicationWindow(std::string title, int width, int height)
    :window(nullptr)
{
    InitWindow(title, width, height);
}

ApplicationWindow::~ApplicationWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();

    if (window)
        delete window;
}

bool Penjin::ApplicationWindow::Update()
{
    if (glfwWindowShouldClose(window))
        return false;
    glfwPollEvents();
    return true;
}

void Penjin::ApplicationWindow::Close()
{
    if(this->window != nullptr)
        glfwDestroyWindow(this->window);
}


void Penjin::ApplicationWindow::GetSize(int* width, int* height)
{
    glfwGetWindowSize(this->window, width, height);
}

int Penjin::ApplicationWindow::GetWidth()
{
    int width, height;
    GetSize(&width, &height);
    return width;
}

int Penjin::ApplicationWindow::GetHeight()
{
    int width, height;
    GetSize(&width, &height);
    return height;
}

void ApplicationWindow::InitWindow(std::string title, int width, int height)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    this->window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (this->window == nullptr)
        throw std::exception(("Unable to initialize window width dimensions [" + std::to_string(width) + " ; " + std::to_string(height) + "]").c_str());

}
