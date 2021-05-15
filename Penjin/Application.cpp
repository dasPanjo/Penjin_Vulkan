#include "Application.h"
#include <iostream>

Application* Application::instance = nullptr;

Application::Application()
	: quit(false)
{
}

void Application::Init(std::string title, int width, int height, bool fullscreen)
{
	InitWindow(title, width, height, fullscreen);

	Start();
	while (!quit)
	{
		Tick();
	}
	Cleanup();
}

void Application::InitWindow(std::string title, int width, int height, bool fullscreen)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::cout << extensionCount << " extensions supported\n";

    glm::mat4 matrix;
    glm::vec4 vec;
    auto test = matrix * vec;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
}
