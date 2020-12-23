#pragma once

#include "RendererCommands.h"

namespace fungine
{
	namespace graphics
	{

		enum class GraphicsAPI
		{
			None,
			OpenGL
		};

		class Graphics
		{
		private:

			static GraphicsAPI s_graphicsAPI;
			static RendererCommands* s_rendererCommands;

		public:

			Graphics();
			~Graphics();

			static void render();

			inline static GraphicsAPI get_graphics_api() { return s_graphicsAPI; }
			inline static void set_graphics_api(GraphicsAPI api) { s_graphicsAPI = api; }
			inline static RendererCommands* const get_renderer_commands() { return s_rendererCommands; }
		};
	}
}