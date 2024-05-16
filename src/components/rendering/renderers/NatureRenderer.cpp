
#include "NatureRenderer.h"
#include "core/window/Window.h"
#include "utils/Time.h"
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

			NatureRenderer::NatureRenderer()
			{
				if (!s_framebuffer)
					s_framebuffer = Framebuffer::create_framebuffer(core::Window::get_width(), core::Window::get_height(), true);

				_entitiesChanged = true;
			}

			NatureRenderer::~NatureRenderer()
			{}

			void NatureRenderer::update()
			{
				if (_entitiesChanged)
				{
					_batches.clear();
					printf("Nature renderer: Entities has changed!\n");
					for (Entity* e : _entities)
						submit(e);

					_entitiesChanged = false;
				}
			}

			void NatureRenderer::submit(Entity* entity)
			{
				std::shared_ptr<Mesh> mesh = entity->getComponent<Mesh>();
				std::shared_ptr<BatchInstanceData> entityBatchData = entity->getComponent<BatchInstanceData>();

				// If this entity haven't been added to a batch yet?
				//	-> Attempt to find a suitable batch for it
				if (!entityBatchData)
				{
					std::shared_ptr<Transform> entityTransform = entity->getComponent<Transform>();
					std::shared_ptr<Material> material = entity->getComponent<Material>();

					for (std::pair<std::shared_ptr<Mesh>, std::shared_ptr<BatchData_NatureRendering>>& batch : _batches)
					{
						if (batch.first == mesh)
						{
							if (batch.second->instanceCount < batch.first->getInstanceCount())
							{
								addToBatch(entity, *batch.second);
								return;
							}
						}
					}

					std::shared_ptr<BatchData_NatureRendering> newBatch = createNewBatch(material, mesh);
					addToBatch(entity, *newBatch);
				}
			}

			void NatureRenderer::render(
				const mml::Matrix4& projectionMatrix,
				const mml::Matrix4& viewMatrix,
				unsigned int renderFlags
			)
			{
				const RendererCommands* rendererCommands = Graphics::get_renderer_commands();
				Camera* camera = Camera::get_current_camera();
				DirectionalLight* directionalLight = DirectionalLight::get_directional_light();
				const ShadowCaster& shadowCaster = directionalLight->getShadowCaster();
				const Framebuffer* shadowmapFramebuffer = shadowCaster.getFramebuffer();

				bool renderGeometry = true;
				bool renderMaterial = renderFlags & RenderFlags::RenderMaterial;
				bool renderLighting = renderFlags & RenderFlags::RenderLighting;
				bool renderShadows =  renderFlags & RenderFlags::RenderShadows;


#ifdef DEBUG__MODE_FULL
				if (!camera)
				{
					Debug::log(
						"Location: void NatureRenderer::render(\n"
						"			const mml::Matrix4 & projectionMatrix, \n"
						"			const mml::Matrix4 & viewMatrix, \n"
						"			unsigned int renderFlags\n"
						"			)\n"
						"Tried to render, but the current camera was nullptr!",
						DEBUG__ERROR_LEVEL__ERROR
					);
					return;
				}
#endif
				for (std::pair<std::shared_ptr<Mesh>, std::shared_ptr<BatchData_NatureRendering>>& batch : _batches)
				{
#ifdef DEBUG__MODE_FULL
					if (!batch.second)
					{
						Debug::log(
							"Location: void NatureRenderer::render(\n"
							"			const mml::Matrix4 & projectionMatrix, \n"
							"			const mml::Matrix4 & viewMatrix, \n"
							"			unsigned int renderFlags\n"
							"			)\n"
							"Mesh of a batch was nullptr!",
							DEBUG__ERROR_LEVEL__ERROR
						);
						continue;
					}
#endif
					Material* material = batch.second->material.get();
					Mesh* mesh = batch.second->mesh.get();
					ShaderProgram* shader = material->getShader();

					// which face we want to cull?
					material->isTwoSided() ? rendererCommands->cullFace(CullFace::None) : rendererCommands->cullFace(CullFace::Back);

					rendererCommands->bindShader(shader);
					if (renderMaterial)
						rendererCommands->bindMaterial(material);

					// Common uniforms
					shader->setUniform("projectionMatrix", projectionMatrix);
					shader->setUniform("viewMatrix", viewMatrix);

					shader->setUniform("directionalLight_direction", directionalLight->getDirection());

					if(shader->hasUniformLocation("cameraPos"))
						shader->setUniform("cameraPos", camera->getEntity()->getComponent<Transform>()->getPosition());

					shader->setUniform("time", (float)Time::get_time());

					shader->setUniform("shadowProjMat", shadowCaster.getProjectionMatrix());
					shader->setUniform("shadowViewMat", shadowCaster.getViewMatrix());

					shader->setUniform("directionalLight_ambientColor", directionalLight->getAmbientColor());
					shader->setUniform("directionalLight_color", directionalLight->getColor());

					shader->setUniform("shadowProperties.shadowmapWidth", shadowmapFramebuffer->getWidth());
					shader->setUniform("shadowProperties.pcfCount", 2);

					// *->TEMP
					shader->setUniform("texture_shadowmap", 4);
					rendererCommands->bindTexture(shadowCaster.getShadowmapTexture(), 4);

					// Finally drawing..
					rendererCommands->bindMesh(mesh);

					const unsigned int instanceCount = batch.second->instanceCount; //mesh->getInstanceCount();


					// Transformation matrices
					if (!batch.second->transformationsBuffUpdated)
					{
						VertexBuffer<float>* instancedMatrixBuffer = mesh->getVertexBuffers()[1];
						instancedMatrixBuffer->update(0, sizeof(float) * 16 * instanceCount, batch.second->transformations);
						batch.second->transformationsBuffUpdated = true;
					}
					else
					{
						batch.second->transformationsBuffUpdated = false;
					}

					// Update the wind instanced data only once
					if (!batch.second->instancedDataHandled)
					{
						// Wind properties
						VertexBuffer<float>* instancedWindBuffer = mesh->getVertexBuffers()[2];
						size_t windPropertyBuffSize = sizeof(float) * instanceCount;
						instancedWindBuffer->update(0, windPropertyBuffSize, batch.second->windProperties);

						//delete[] batch.second->transformations;
						//delete[] batch.second->windProperties;

						batch.second->instancedDataHandled = true;
					}

					rendererCommands->drawIndices_instanced(mesh, instanceCount);
					rendererCommands->unbindMesh(mesh);

					// Unbind shadowmap texture
					rendererCommands->unbindTexture(shadowCaster.getShadowmapTexture(), 4);

					if (renderMaterial)
						rendererCommands->unbindMaterial(material);

					rendererCommands->unbindShader();
				}
			}


			void NatureRenderer::renderShadows()
			{
				const RendererCommands* rendererCommands = Graphics::get_renderer_commands();
				const ShadowCaster& shadowCaster = DirectionalLight::get_directional_light()->getShadowCaster();

				for (std::pair<std::shared_ptr<Mesh>, std::shared_ptr<BatchData_NatureRendering>>& batch : _batches)
				{
#ifdef DEBUG__MODE_FULL
					if (!batch.second)
					{
						Debug::log(
							"Location: void NatureRenderer::renderShadows()\n"
							"Mesh of a batch was nullptr!",
							DEBUG__ERROR_LEVEL__ERROR
						);
						continue;
					}
#endif
					Material* material = batch.second->material.get();
					ShaderProgram* shader = material->getShadowShader();
					Mesh* mesh = batch.second->mesh.get();
					if (!shader || !mesh->hasShadows())
						continue;

					rendererCommands->bindShader(shader);

					// Common uniforms
					shader->setUniform("projectionMatrix", shadowCaster.getProjectionMatrix());
					shader->setUniform("viewMatrix", shadowCaster.getViewMatrix());

					// Just a quick hack for now..
					ShaderUniform<float>* uniform_windMultiplier = material->getShaderUniform_Float("m_windMultiplier");
					if(uniform_windMultiplier)
						shader->setUniform("m_windMultiplier", uniform_windMultiplier->data);

					shader->setUniform("time", (float)Time::get_time());

					// Drawing..
					rendererCommands->bindMesh(mesh);

					const unsigned int instanceCount = batch.second->instanceCount;//mesh->getInstanceCount();

					// Transformation matrices
					if (!batch.second->transformationsBuffUpdated)
					{
						VertexBuffer<float>* instancedMatrixBuffer = mesh->getVertexBuffers()[1];
						instancedMatrixBuffer->update(0, sizeof(float) * 16 * instanceCount, batch.second->transformations);
						batch.second->transformationsBuffUpdated = true;
					}
					else
					{
						batch.second->transformationsBuffUpdated = false;
					}

					// Update the wind instanced data only once
					if (!batch.second->instancedDataHandled)
					{
						// Wind properties
						VertexBuffer<float>* instancedWindBuffer = mesh->getVertexBuffers()[2];
						size_t windPropertyBuffSize = sizeof(float) * instanceCount;
						instancedWindBuffer->update(0, windPropertyBuffSize, batch.second->windProperties);

						//delete[] batch.second->transformations;
						//delete[] batch.second->windProperties;

						batch.second->instancedDataHandled = true;
					}

					rendererCommands->drawIndices_instanced(mesh, instanceCount);
					rendererCommands->unbindMesh(mesh);

					rendererCommands->unbindShader();
				}
			}

			void NatureRenderer::clear()
			{
				//_batches.clear();
			}

			std::shared_ptr<BatchData_NatureRendering> NatureRenderer::createNewBatch(std::shared_ptr<Material>& material, std::shared_ptr<Mesh>& mesh)
			{
				printf("Creating new batch!\n");

				std::shared_ptr<BatchData_NatureRendering> newBatch = std::make_shared<BatchData_NatureRendering>(material, mesh->getInstanceCount());
				newBatch->mesh = Mesh::create_copy(mesh);

				// Create the actual vertex buffer for that..
				int transformationsBuffLength = 16 * newBatch->mesh->getInstanceCount();
				VertexBufferLayout transformationsBuffLayout({
					{ 4, ShaderDataType::Float4, true },
					{ 5, ShaderDataType::Float4, true },
					{ 6, ShaderDataType::Float4, true },
					{ 7, ShaderDataType::Float4, true }
				});
				VertexBuffer<float>* perInstanceTransformationsBuffer = VertexBuffer<float>::create_vertex_buffer(
					newBatch->transformations,
					sizeof(float) * transformationsBuffLength,
					BufferUsage::StreamDraw,
					transformationsBuffLayout
				);

				// Create per instance "wind properties buffer"
				int windPropertiesBuffLength = newBatch->mesh->getInstanceCount();
				VertexBufferLayout windBufferLayout({
					{ 8, ShaderDataType::Float, true }
				});

				VertexBuffer<float>* perInstanceWindBuffer = VertexBuffer<float>::create_vertex_buffer(
					newBatch->windProperties,
					sizeof(float) * windPropertiesBuffLength,
					BufferUsage::StaticDraw,
					windBufferLayout
				);

				// *When adding vertex buffer to a mesh, we need to have the mesh bound,
				//	..quite fucking stupid and annoying "mesh binding system thing..
				//	*->TODO: Do something about mesh binding shit thing
				RendererCommands* rc = Graphics::get_renderer_commands();
				rc->bindMesh(newBatch->mesh.get());
				newBatch->mesh->addVertexBuffer(perInstanceTransformationsBuffer);
				newBatch->mesh->addVertexBuffer(perInstanceWindBuffer);
				rc->unbindMesh(newBatch->mesh.get());

				_batches.push_back(std::make_pair(mesh, newBatch));
				return newBatch;
			}

			void NatureRenderer::addToBatch(entities::Entity* entity, BatchData_NatureRendering& batch)
			{
				std::shared_ptr<Transform> transform = entity->getComponent<Transform>();

				float* buffToTrack = transform->getTransformationMatrixBuff();

				std::shared_ptr<BatchInstanceData> entityBatchData = std::make_shared<BatchInstanceData>(
					batch.instanceCount,
					_batches.size() - 1,
					&batch.transformations,
					buffToTrack,
					16
				);
				entity->addComponent(entityBatchData);

				transform->update();
				addToTransformsBuff(batch, transform->getTransformationMatrix());
				addToWindInitValsBuff(batch);
				batch.instanceCount++;
			}

			// *May be wrong!?
			void NatureRenderer::addToTransformsBuff(BatchData_NatureRendering& batch, const mml::Matrix4& transformationMatrix)
			{
				unsigned int& ptr = batch.staticArrDataPtr_transformations;
				if (ptr < batch.mesh->getInstanceCount() * 16)
				{
					for (int i = 0; i < 16; ++i)
						batch.transformations[ptr + i] = transformationMatrix[i];

					ptr += 16;
				}
			}
			void NatureRenderer::addToWindInitValsBuff(BatchData_NatureRendering& batch)
			{
				unsigned int& ptr = batch.staticArrDataPtr_wind;
				if (ptr < batch.mesh->getInstanceCount())
				{
					batch.windProperties[ptr] = (float)(std::rand() % 100) * 0.1f;
					ptr++;
				}
			}

			const size_t NatureRenderer::getSize() const
			{
				return sizeof(*this);
			}
		}
	}
}
