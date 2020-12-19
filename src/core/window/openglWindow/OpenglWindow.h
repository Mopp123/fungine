#pragma once

#include <glfw3.h>
#include "core/window/Window.h"

namespace fungine
{
	namespace core
	{
		class OpenglWindow : public Window
		{
		private:

			GLFWwindow* _glfwWindowHandle = nullptr;

		public:

			OpenglWindow(const char* title, const int& width, const int& height, const int& swapInterval, const int& MSAASamples, bool fullscreen);
			~OpenglWindow();


			virtual bool isCloseRequested() override;
			virtual void close()			override;
			virtual void cleanUp()			override;

			virtual void swapBuffers() const override;
			virtual void updateEvents() override;

			// Creates input mappings, for key names, actions, etc from windowing api format to our format.
			virtual void createInputMapping() const override;
		};
	}
}