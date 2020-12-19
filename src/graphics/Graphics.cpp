
#include "Graphics.h"

namespace fungine
{
	namespace graphics
	{
		GraphicsAPI Graphics::s_graphicsAPI = GraphicsAPI::OpenGL;
		RendererCommands* Graphics::s_rendererCommands = nullptr;


		Graphics::Graphics()
		{
			s_rendererCommands = RendererCommands::create_renderer_commands();
		}

		Graphics::~Graphics()
		{
			delete s_rendererCommands;
		}
	}
}