
#include "Program.h"
#include "Common.h"
#include "utils/Time.h"
#include "Debug.h"

#include <assert.h>

#define NDEBUG 

namespace fungine
{
	namespace core
	{

		std::unique_ptr<Window> Program::_window = nullptr;

		Program::Program(const std::string& windowTitle, int windowWidth, int windowHeight, bool fullscreen, int swapInterval)
		{
			graphics::Graphics::set_graphics_api(graphics::GraphicsAPI::OpenGL);
			_window = Window::create_window(windowTitle.c_str(), windowWidth, windowHeight, swapInterval, 2, fullscreen);
			
			// This is fucking retarded..
			_window->createInputMapping();

			assert(_window != nullptr);
		}

		Program::~Program()
		{}

		bool Program::isCloseRequested() const
		{
			return _window->isCloseRequested();
		}

		using namespace components;
		void Program::update()
		{
			// !!!!!!!!!!!!!! This is done just temporarely here !!!!!!!!!!!!!!!!!! *->TEMP
			for (Component* c : Component::s_allComponentsPool) // *NOTE: remember the "pointer invalidation shit thing" if u try to access this vector, if it has changed in this update at some point.. then all those pointers are inaccessable..
			{
				size_t dynamicComponentCount = Component::s_allComponentsPool.size();
				if (c)
				{
					if (c->_isActive)
						c->update();
				}
			}
			_window->swapBuffers();
			_window->updateEvents();
			Time::update();
		}
	}
}