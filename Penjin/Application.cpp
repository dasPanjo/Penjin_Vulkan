#include "Application.h"
#include <iostream>
#include <vector>

#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "VulkanManager.h"

using namespace Penjin;

Application* Application::instance = nullptr;

Application::Application(std::string name, uint32_t version)
	:
	name(name),
	version(version),
	quit(false),
	window(nullptr)
{
}

Application::~Application() {
}

int Application::Run() {
	int result = EXIT_SUCCESS;
	try
	{
		Start();
		while (!quit)
		{
			if (!glfwWindowShouldClose(window->GetHandle()))
				glfwPollEvents();
			else {
				Quit();
				break;
			}
			Tick();
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		auto c = getchar();
		result = EXIT_FAILURE;
	}
	Cleanup();
	return result;
}

void Penjin::Application::Cleanup()
{
	VulkanManager::Cleanup();
}

bool Application::Init(int width, int height)
{
	try {
		window = new ApplicationWindow(this->name, width, height);
		VulkanManager::Init(this->name, this->version);
		return true;
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Unknown error during application initialization" << std::endl;
	}
	if (this->window != nullptr)
		this->window->Close();
	return false;
}

