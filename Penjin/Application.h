#pragma once
#include <string>
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

namespace Penjin {
	class Application
	{
	public:

		Application(std::string title, int width, int height);
		virtual ~Application();
		static void DeleteInstance() { if (instance) { delete instance; } }

		virtual int Run();
		inline virtual void Quit() { quit = true; }

		Application* GetInstance() const { return instance; }

	protected:
		/**Absract methods **/
		virtual void Start() abstract;
		virtual void Tick() abstract;
		virtual void Cleanup() abstract;

		/**Init**/
		virtual bool Init();
		virtual bool InitWindow(int width, int height);
		virtual bool InitVulkan();

		/**Instance**/
		static Application* instance;

		/**Window settings**/
		std::string title;
		int width;
		int height;

		/**GLFW**/
		GLFWwindow* window;

		/**Vulkan**/
		VkApplicationInfo vkAppInfo;
		VkInstanceCreateInfo vkInstanceCreateInfo;

		/**Application status**/
		bool quit;

	};
}
