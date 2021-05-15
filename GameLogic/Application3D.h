#pragma once

#include <Application.h>
class Application3D : public Application
{
public:
	Application3D();
	virtual ~Application3D() override;

	static void Run() { DeleteInstance(); instance = new Application3D(); instance->Init("Game Logic", 1280, 720, false); }
	

	virtual void Start() override;
	virtual void Tick() override;
	virtual void Cleanup() override;

protected:
	int counter;
};

