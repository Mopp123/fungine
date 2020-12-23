
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


			void TerrainRenderer::submit(Entity* e)
			{
				std::shared_ptr<Material> entityMaterial = e->getComponent<Material>();
				for (std::pair<std::shared_ptr<Material>, std::vector<Entity*>>& batch : _batches)
				{
					if (batch.first == entityMaterial)
					{
						batch.second.push_back(e);
						return;
					}
				}

				std::vector<Entity*> newBatch;
				newBatch.push_back(e);
				_batches.push_back(std::make_pair(entityMaterial, newBatch));
			}

			void TerrainRenderer::flush(
				const mml::Matrix4& projectionMatrix,
				const mml::Matrix4& viewMatrix
			)
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
					return;
				}
#endif
				for (std::pair<std::shared_ptr<Material>, std::vector<Entity*>>& batch : _batches)
				{
					Material* material = batch.first.get();
					ShaderProgram* shader = material->getShader();

					rendererCommands->bindMaterial(material);

					// common uniforms
					shader->setUniform("projectionMatrix", projectionMatrix);
					shader->setUniform("viewMatrix", viewMatrix);
					shader->setUniform("cameraPos", camera->getEntity()->getComponent<Transform>()->getPosition());
					shader->setUniform("directionalLight_direction", directionalLight->getDirection());
					shader->setUniform("directionalLight_ambientColor", directionalLight->getAmbientColor());
					shader->setUniform("directionalLight_color", directionalLight->getColor());

					// per entity inside the batch
					for (Entity* e : batch.second)
					{
						std::shared_ptr<Mesh> mesh = e->getComponent<Mesh>();
						// per entity uniforms
						shader->setUniform("transformationMatrix", e->getComponent<Transform>()->getTransformationMatrix());

						// Finally drawing..
						rendererCommands->bindMesh(mesh.get());
						rendererCommands->drawIndices(mesh.get());
						rendererCommands->unbindMesh(mesh.get());
					}

					rendererCommands->unbindMaterial(material);
				}
			}


			void TerrainRenderer::clear()
			{
				_batches.clear();
			}

			const size_t TerrainRenderer::getSize() const
			{
				return sizeof(*this);
			}
		}
	}
}