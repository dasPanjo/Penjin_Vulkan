#pragma once

#include <Application.h>

using namespace Penjin;

class Application3D : public Application
{
public:
	static int Run(std::string title, uint32_t version, int width, int height);

	Application3D(std::string title, uint32_t version);
	virtual ~Application3D() override { Application::~Application(); }

	virtual void Start() override;
	virtual void Tick() override;
	virtual void Cleanup() override;

protected:
};

