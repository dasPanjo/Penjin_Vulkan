#include "Application3D.h"
#include <iostream>

Application3D::Application3D(std::string title, uint32_t version) :
	Application(title, version)
{
}


int Application3D::Run(std::string title, uint32_t version, int width, int height)
{
	DeleteInstance();
	instance = new Application3D(title, version);
	if (!instance->Init(width, height))
		return EXIT_FAILURE;
	return instance->Run();
}

void Application3D::Start()
{
}

void Application3D::Tick()
{
}

void Application3D::Cleanup()
{
	Application::Cleanup();
	std::cout << "Cleanup!";
}
