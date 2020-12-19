#pragma once

#include "window/Window.h"
#include "graphics/Graphics.h"

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

			Program();
			~Program();
			bool isCloseRequested() const;

			void update();

			inline static const std::unique_ptr<Window>& get_window() { return _window; }
		};
	}
}