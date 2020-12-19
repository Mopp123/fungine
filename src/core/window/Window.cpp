
#include "Window.h"
#include "graphics/Graphics.h"
#include "openglWindow/OpenglWindow.h"
#include "core/Debug.h"
#include <stdio.h>

namespace fungine
{
	namespace core
	{
		int Window::s_width = DEFAULT__WINDOW_WIDTH;
		int Window::s_height = DEFAULT__WINDOW_HEIGHT;

		Window::Window(
			const char* title,
			const int& width, const int& height,
			const int& swapInterval, const int& MSAASamples,
			bool fullscreen
		) :
			_swapInterval(swapInterval), _MSAASamples(MSAASamples), _isFullscreen(fullscreen)
		{
			s_width = width;
			s_height = height;
		}

		Window::~Window()
		{
			Debug::notify_on_destroy("Window(core)");
		}


		std::unique_ptr<Window> Window::create_window(
			const char* title,
			const int& width, const int& height,
			const int& swapInterval, const int& MSAASamples,
			bool fullscreen
		)
		{
			switch (graphics::Graphics::get_graphics_api())
			{
			case graphics::GraphicsAPI::OpenGL: return std::make_unique<OpenglWindow>(
				title,
				width, height,
				swapInterval, MSAASamples,
				fullscreen
				);
			default:
				Debug::log(
					"Location: std::shared_ptr<Window> Window::create_window(\n"
					"const char* title,\n"
					"const int& width, const int& height,\n"
					"const int& swapInterval, const int& MSAASamples,\n"
					"bool fullscreen = false\n"
					")\n"
					"Invalid graphics api was in use when attempting to create window.",
					DEBUG__ERROR_LEVEL__FATAL_ERROR
				);
				break;
			}

			return nullptr;
		}
	}
}