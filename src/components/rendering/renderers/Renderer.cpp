
#include "Renderer.h"
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

			void RenderQue::add(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
			{
				// If you can find already existing batch for this mesh/material combo -> add it to that batch
				if (_batches.find(material) != _batches.end())
				{
					std::vector<std::shared_ptr<Mesh>>& batch = _batches[material];
				#ifdef DEBUG__MODE_FULL
					if (batch.empty())
					{
						Debug::log(
							"Location: void RenderQue::add(const Mesh* mesh, const Material* material)\n"
							"Triend to add mesh to an already existing batch, but the batch was empty!\n"
							"If there is a batch there should always be at least 1 mesh in its' render que.",
							DEBUG__ERROR_LEVEL__ERROR
						);
						return;
					}
				#endif
					add_to_dynamic_list(batch, mesh);
				}
				else // If we cannot find already existing batch for
				{
					std::vector<std::shared_ptr<Mesh>> newBatch;
					add_to_dynamic_list(newBatch, mesh);
					_batches.insert(std::make_pair(material, newBatch));
				}
			}

			void RenderQue::clear()
			{
				_batches.clear();
			}


			std::vector<entities::Entity*> Renderer::s_entities; // Entities this same renderer has been attached to
			unsigned int Renderer::s_processedEntities;
			RenderPass Renderer::s_renderPass;

			bool Renderer::s_isReadyToFlush = false;

			Framebuffer* Renderer::s_framebuffer = nullptr;

			Renderer::Renderer() :
				Component(nullptr)
			{
				if (!s_framebuffer)
					s_framebuffer = Framebuffer::create_framebuffer(core::Window::get_width(), core::Window::get_height(), true);
			}

			Renderer::~Renderer()
			{
				s_entities.erase(std::find(s_entities.begin(), s_entities.end(), _entity));
			}

			void Renderer::onAttackToEntity(entities::Entity* entity)
			{
				add_to_dynamic_list(s_entities, entity);
			}

			void Renderer::update()
			{
				submit(_entity);
				s_processedEntities++;

				s_isReadyToFlush = s_processedEntities == s_entities.size();

				if (s_isReadyToFlush)
					flush();
			}

			void Renderer::submit(entities::Entity* entity)
			{
				std::shared_ptr<Mesh> mesh = entity->getComponent<Mesh>();
				std::shared_ptr<Material> material = entity->getComponent<Material>();
				if(mesh && material)
					s_renderPass.renderQue.add(mesh, material);
			}


			void Renderer::flush()
			{
				const RendererCommands* rendererCommands = Graphics::get_renderer_commands();
				Camera* camera = Camera::get_current_camera();
				DirectionalLight* directionalLight = DirectionalLight::get_directional_light();

			#ifdef DEBUG__MODE_FULL
				if (!camera)
				{
					Debug::log(
						"Location: void Renderer::flush()\n"
						"Tried to flush renderer, but the current camera was nullptr!",
						DEBUG__ERROR_LEVEL__ERROR
					);
					clear();
					return;
				}
			#endif


				// * for each render pass in the future
				rendererCommands->bindFramebuffer(s_framebuffer);

				for (auto& batch : s_renderPass.renderQue.getBatches())
				{
					Material* material = batch.first.get();
					ShaderProgram* shader = material->getShader();

					rendererCommands->bindMaterial(material);

					// "bind texture units"
					shader->setUniform("texture_diffuse", 0);

					if (material->hasSpecularMap())
						shader->setUniform("texture_specular", 1);

					if (material->hasNormalMap())
						shader->setUniform("texture_normal", 2);

					// commong uniforms
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

			void Renderer::clear()
			{
				s_isReadyToFlush = false;
				s_processedEntities = 0;
				s_renderPass.renderQue.clear();
			}

			const size_t Renderer::getSize() const
			{
				return sizeof(*this);
			}
		}
	}
}