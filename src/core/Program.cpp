
#include "Program.h"
#include "Common.h"
#include "Debug.h"

#include <assert.h>

#define NDEBUG 

namespace fungine
{
	namespace core
	{

		std::unique_ptr<Window> Program::_window = nullptr;

		Program::Program()
		{
			graphics::Graphics::set_graphics_api(graphics::GraphicsAPI::OpenGL);
			_window = Window::create_window("Opengl testing", DEFAULT__WINDOW_WIDTH, DEFAULT__WINDOW_HEIGHT, 0, 1);
			
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
		}
	}
}