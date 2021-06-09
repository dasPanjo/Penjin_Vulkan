#include "Application.h"
#include <iostream>
#include <vector>

#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "VulkanManager.h"
#include "Log.h"

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
			if(!window->Update())
			{
				Quit();
				break;
			}
			Tick();
		}
	}
	catch (const std::exception& e)
	{
		Log::Critical(e.what());
		auto c = getchar();
		result = EXIT_FAILURE;
	}
	Cleanup();
	return result;
}

void Penjin::Application::Cleanup()
{
	if (window != nullptr)
		window->Close();
	VulkanManager::Cleanup();
}

bool Application::Init(int width, int height)
{
	try {
		window = new ApplicationWindow(this->name, width, height);
		VulkanManager::Init(this->name, this->version);
		return true;
	}
	catch (const std::exception& e) {
		Log::Critical(e.what());
	}
	catch (...) {
		Log::Critical("Unknown error during application initialization");
	}
	Cleanup();
	return false;
}

