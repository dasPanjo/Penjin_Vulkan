#pragma once
#include <string>
#include <GLFW/glfw3.h>
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"

class Application
{
public:

	Application();
	virtual ~Application() { Application::DeleteInstance(); }

	static void DeleteInstance() { if (instance) { delete instance; } }

	virtual void Init(std::string title, int width, int height, bool fullscreen);

	virtual void Start() abstract;
	virtual void Tick() abstract;
	virtual void Cleanup() abstract;

	inline virtual void Quit() { quit = true; }

protected:
	virtual void InitWindow(std::string title, int width, int height, bool fullscreen);

	static Application* instance;
	bool quit;

};
