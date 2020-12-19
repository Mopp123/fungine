
#include "RendererCommands.h"
#include "Graphics.h"
#include "opengl/OpenglRendererCommands.h"

#include "core/Debug.h"

namespace fungine
{
	namespace graphics
	{
		// We allow RenderCommands creation only through the function "create_render_commands".
		//	-> make operator new inaccessable for outsiders
		void* RendererCommands::operator new(size_t size)
		{
			return malloc(size);
		}

		RendererCommands* RendererCommands::create_renderer_commands()
		{
			switch (Graphics::get_graphics_api())
			{
			case GraphicsAPI::OpenGL: return new opengl::OpenglRendererCommands;
			default:
				Debug::log(
					"Location: RendererCommands* RendererCommands::create_renderer_commands()\n"
					"Invalid graphics api was in use.",
					DEBUG__ERROR_LEVEL__FATAL_ERROR
				);
				break;
			}

			return nullptr;
		}

	}
}