#pragma once

#include "input/InputHandler.h"
#include <memory>

namespace fungine
{
	namespace core
	{
		class Window
		{
		protected:

			static int s_width;
			static int s_height;
			int _swapInterval;
			int _MSAASamples;
			bool _isFullscreen;

			bool _isCloseRequested = false;

			InputHandler _inputHandler;

		protected:

			Window(
				const char* title,
				const int& width, const int& height,
				const int& swapInterval, const int& MSAASamples,
				bool fullscreen
			);

		public:

			virtual ~Window();

			static std::unique_ptr<Window> create_window(
				const char* title,
				const int& width, const int& height,
				const int& swapInterval, const int& MSAASamples,
				bool fullscreen = false
			);

			inline static int get_width()	{ return s_width; }
			inline static int get_height()	{ return s_height; }

			inline const int& getSwapInterval() const { return _swapInterval; }
			inline const int& getMSAASamples()	const { return _MSAASamples; }
			inline const int& isFullscreen()	const { return _isFullscreen; }

			inline InputHandler& getInputHandler() { return _inputHandler; }

			virtual bool isCloseRequested() = 0;
			virtual void close() = 0;
			virtual void cleanUp() = 0;

			virtual void swapBuffers() const = 0;
			virtual void updateEvents() = 0;
			
			// Creates input mappings, for key names, actions, etc from windowing api format to our format.
			virtual void createInputMapping() const = 0;
		};
	}
}