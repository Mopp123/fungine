
#include "Graphics.h"
#include "components/rendering/renderers/Renderer.h"
#include "components/rendering/Camera.h"
#include "components/rendering/lighting/Lights.h"


namespace fungine
{
	using namespace components;
	using namespace rendering;

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

		void Graphics::render()
		{
		
			// *->TEMP

			Camera* camera = Camera::get_current_camera();
			DirectionalLight* directionalLight = DirectionalLight::get_directional_light();
			ShadowCaster& shadowCaster = directionalLight->getShadowCaster();

			// Render shadowmap
			Framebuffer* shadowmapFramebuffer = directionalLight->getShadowCaster().getFramebuffer();
			
			s_rendererCommands->bindFramebuffer(shadowmapFramebuffer);
			s_rendererCommands->clear();

			for (Renderer* renderer : Renderer::get_all_renderers())
			{
				if (renderer->rendersShadows())
				{
					renderer->flush(
						shadowCaster.getProjectionMatrix(),
						shadowCaster.getViewMatrix(),
						RenderFlags::RenderGeometry |
						RenderFlags::RenderMaterial
					);
				}
			}
			// Render to screen
			Framebuffer* screenFramebuffer = Renderer::get_screen_framebuffer();

			s_rendererCommands->bindFramebuffer(screenFramebuffer);
			s_rendererCommands->clear();
			for (Renderer* renderer : Renderer::get_all_renderers())
				renderer->flush(
					camera->getProjectionMatrix(),
					camera->getViewMatrix(),
					RenderFlags::RenderGeometry |
					RenderFlags::RenderMaterial |
					RenderFlags::RenderLighting |
					RenderFlags::RenderShadows
				);


			// Finally clear all renderers.. (currently this clears all dynamic renderLists/batches of the renderers..)
			for (Renderer* renderer : Renderer::get_all_renderers())
				renderer->clear();

		}
	}
}