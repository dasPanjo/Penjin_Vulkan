#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
namespace Penjin {

class ApplicationWindow
{
public:
	ApplicationWindow(std::string title, int width, int height);
	virtual ~ApplicationWindow();

	bool Update();
	void Close();

	std::string GetTitle() const { return title; }
	void GetSize(int* width, int* height);
	int GetWidth();
	int GetHeight();

	GLFWwindow* GetHandle() const { return window; }

private:
	void InitWindow(std::string title, int width, int height);
	GLFWwindow* window;
	std::string title;
};

}
