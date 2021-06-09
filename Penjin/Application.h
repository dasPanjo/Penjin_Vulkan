#pragma once
#include "ApplicationWindow.h"

#include <string>

namespace Penjin {
	class Application
	{
	public:
		Application(std::string name, uint32_t version);
		virtual ~Application();
		static void DeleteInstance() { if (instance) { delete instance; } }

		virtual bool Init(int width, int height);
		virtual int Run();
		inline virtual void Quit() { quit = true; }

		Application* GetInstance() const { return instance; }

	protected:
		/**Absract methods **/
		virtual void Start() abstract;
		virtual void Tick() abstract;
		virtual void Cleanup();


		/**Instance**/
		static Application* instance;

		/**Application information**/
		std::string name;
		uint32_t version;

		/**Window**/
		ApplicationWindow* window;

		/**Application status**/
		bool quit;

	};
}
