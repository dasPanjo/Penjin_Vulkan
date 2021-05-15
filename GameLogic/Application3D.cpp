#include "Application3D.h"
#include <iostream>



Application3D::Application3D()
	:counter(0)
{
}

Application3D::~Application3D()
{
	Cleanup();
}

void Application3D::Start()
{
	std::cout << "Hello ";
}

void Application3D::Tick()
{
	counter++;
	std::cout << " World" << std::endl;
	getchar();
	if (counter > 3)
		Quit();
}

void Application3D::Cleanup()
{
	std::cout << "Bye!";
	getchar();
}
