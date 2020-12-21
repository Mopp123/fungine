#pragma once

#include "window/Window.h"
#include "graphics/Graphics.h"
#include <string>

namespace fungine
{
	namespace core
	{
		class Program
		{
		private:

			static std::unique_ptr<Window> _window;

			graphics::Graphics _graphics;

		public:

			Program(const std::string& windowTitle, int windowWidth, int windowHeight);
			~Program();
			bool isCloseRequested() const;

			void update();

			inline static const std::unique_ptr<Window>& get_window() { return _window; }
		};
	}
}