
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
			unsigned int updated = 0;
			// !!!!!!!!!!!!!! This is done just temporarely here !!!!!!!!!!!!!!!!!! *->TEMP
			for (int i = 0; i < Component::s_updateableComponentsPool.size(); ++i) // *NOTE: remember the vector iterator/ptr/ref invalidation when the vector's size changes
			{
				Component* c = Component::s_updateableComponentsPool[i];
				if (c)
				{
					if (c->_isActive)
					{
						c->update();
						updated++;
					}
				}
			}
			_window->swapBuffers();
			_window->updateEvents();
			Time::update();
		}
	}
}