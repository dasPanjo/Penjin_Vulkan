#include "Application.h"
#include <iostream>
#include <vector>

#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"


namespace Penjin {

    Application* Application::instance = nullptr;

    Application::Application(std::string title, int width, int height)
        : quit(false),
        window(nullptr),
        title(title),
        width(width),
        height(height)
    {
    }

    Application::~Application() {

        glfwDestroyWindow(window);
        glfwTerminate();

        if(window) 
            delete window;

        Application::DeleteInstance();
    }
    
    int Application::Run() {
        int result = EXIT_SUCCESS;
        try 
        {
            if (!Init()) 
            {
                throw new std::exception("Unknown initialization error!");
            }

            Start();
            while (!quit)
            {
                if (!glfwWindowShouldClose(window))
                    glfwPollEvents();
                else {
                    Quit();
                    break;
                }
                Tick();
            }
        }
        catch (const std::exception &e) 
        {
            std::cerr << e.what() << std::endl;
            auto c = getchar();
            result = EXIT_FAILURE;
        }
        Cleanup();
        return result;
    }

    bool Application::Init()
    {
        if (!InitWindow(width, height))
            return false;
        if (!InitVulkan())
            return false;
        return true;
    }

    bool Application::InitWindow(int width, int height)
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        this->window = glfwCreateWindow(width, height, this->title.c_str(), nullptr, nullptr);
        if (this->window == nullptr)
            return false;

        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::cout << extensionCount << " extensions supported\n";

        return true;
    }

    bool Application::InitVulkan()
    {
        return true;
    }

}
