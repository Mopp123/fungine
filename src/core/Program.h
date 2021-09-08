#pragma once

#include "window/Window.h"
#include "graphics/Graphics.h"
#include <string>

namespace fungine
{
	namespace core
	{

		/// "Interface" between the engine and your application code
		/// 
		/// This is required to use functionality of the engine.
		/// Creates window, rendering context, handles input, scenes etc.
		class Program
		{
		private:

			static std::unique_ptr<Window> _window;

			graphics::Graphics _graphics;

		public:

			Program(const std::string& windowTitle, int windowWidth, int windowHeight, bool fullscreen = false, int swapInterval = 1);
			~Program();
			bool isCloseRequested() const;

			void update();

			inline static const std::unique_ptr<Window>& get_window() { return _window; }
		};
	}
}