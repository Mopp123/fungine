
#include "TerrainRenderer.h"
#include "core/window/Window.h"
#include "graphics/Graphics.h"
#include "graphics/RendererCommands.h"

#include "components/rendering/Camera.h"
#include "components/rendering/lighting/Lights.h"

#include "core/Debug.h"

namespace fungine
{
	using namespace entities;
	using namespace graphics;

	namespace components
	{
		namespace rendering
		{

			TerrainRenderer::TerrainRenderer()
			{
				if (!s_framebuffer)
					s_framebuffer = Framebuffer::create_framebuffer(core::Window::get_width(), core::Window::get_height(), true);
			}

			TerrainRenderer::~TerrainRenderer()
			{}

			void TerrainRenderer::flush()
			{
				const RendererCommands* rendererCommands = Graphics::get_renderer_commands();
				Camera* camera = Camera::get_current_camera();
				DirectionalLight* directionalLight = DirectionalLight::get_directional_light();

#ifdef DEBUG__MODE_FULL
				if (!camera)
				{
					Debug::log(
						"Location: void TerrainRenderer::flush()"
						"Tried to flush renderer, but the current camera was nullptr!",
						DEBUG__ERROR_LEVEL__ERROR
					);
					clear();
					return;
				}
#endif


				// * for each render pass in the future
				rendererCommands->bindFramebuffer(s_framebuffer);

				for (auto& batch : _renderPass.renderQue.getBatches())
				{
					Material* material = batch.first.get();
					ShaderProgram* shader = material->getShader();

					rendererCommands->bindMaterial(material);

					// common uniforms
					shader->setUniform("projectionMatrix", camera->getProjectionMatrix());
					shader->setUniform("viewMatrix", camera->getViewMatrix());
					shader->setUniform("cameraPos", camera->getEntity()->getComponent<Transform>()->getPosition());
					shader->setUniform("directionalLight_direction", directionalLight->getDirection());
					shader->setUniform("directionalLight_ambientColor", directionalLight->getAmbientColor());
					shader->setUniform("directionalLight_color", directionalLight->getColor());

					// per entity inside the batch
					for (int i = 0; i < batch.second.size(); ++i)
					{
						Entity* entity = batch.second[i]->getEntity();

						std::shared_ptr<Mesh> mesh = entity->getComponent<Mesh>();
						// per entity uniforms
						shader->setUniform("transformationMatrix", entity->getComponent<Transform>()->getTransformationMatrix());

						// Finally drawing..
						rendererCommands->bindMesh(mesh.get());
						rendererCommands->drawIndices(mesh.get());
						rendererCommands->unbindMesh(mesh.get());
					}

					rendererCommands->unbindMaterial(material);
				}
				rendererCommands->unbindFramebuffer(nullptr);

				clear();
			}

			const size_t TerrainRenderer::getSize() const
			{
				return sizeof(*this);
			}
		}
	}
}