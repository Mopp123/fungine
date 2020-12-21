
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

		Program::Program(const std::string& windowTitle, int windowWidth, int windowHeight)
		{
			graphics::Graphics::set_graphics_api(graphics::GraphicsAPI::OpenGL);
			_window = Window::create_window(windowTitle.c_str(), windowWidth, windowHeight, 1, 1);
			
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
			// !!!!!!!!!!!!!! This is just temporarely here !!!!!!!!!!!!!!!!!! *->TEMP
			for (Component* c : Component::s_allComponentsPool)
			{
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