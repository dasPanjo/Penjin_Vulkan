#include "Application3D.h"
#include <iostream>

Application3D::Application3D(std::string title, int width, int height) :
	Application(title, width, height)
{
}


int Application3D::Run(std::string title, int width, int height)
{
	DeleteInstance(); 
	instance = new Application3D(title, width, height);
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
	std::cout << "Cleanup!";
}
