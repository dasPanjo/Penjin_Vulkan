#pragma once

#include <Application.h>

using namespace Penjin;

class Application3D : public Application
{
public:
	Application3D(std::string title, int width, int height);
	virtual ~Application3D() override { Application::~Application(); }

	static int Run(std::string title, int width, int height);
	
	virtual void Start() override;
	virtual void Tick() override;
	virtual void Cleanup() override;

protected:
};

