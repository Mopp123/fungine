
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
			}

			NatureRenderer::~NatureRenderer()
			{}
			
			void NatureRenderer::update()
			{
				if (!_lockSubmitting)
				{
					for (Entity* e : _entities)
						submit(e);

					_lockSubmitting = true;
				}
			}

			void NatureRenderer::flush(
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
						"Location: void NatureRenderer::flush()"
						"Tried to flush renderer, but the current camera was nullptr!",
						DEBUG__ERROR_LEVEL__ERROR
					);
					return;
				}
#endif
				for (std::pair<std::shared_ptr<Mesh>, std::shared_ptr<BatchData>>& batch : _batches)
				{
					Material* material = batch.second->material.get();
					Mesh* mesh = batch.second->mesh.get();
					const unsigned int meshInstanceCount = mesh->getInstanceCount();

					ShaderProgram* shader = material->getShader();

					rendererCommands->bindMaterial(material);

					// "bind texture units"
					shader->setUniform("texture_diffuse", 0);

					if (material->hasSpecularMap())
						shader->setUniform("texture_specular", 1);

					if (material->hasNormalMap())
						shader->setUniform("texture_normal", 2);


					float aspectRatio = (float)(core::Window::get_width()) / (float)(core::Window::get_height());
					mml::Matrix4 projMat(1.0f);
					mml::create_perspective_projection_matrix(projMat, 70.0f, aspectRatio, 0.1f, 2.5f);
					// common uniforms
					shader->setUniform("projectionMatrix", projectionMatrix);
					shader->setUniform("viewMatrix", viewMatrix);
					shader->setUniform("cameraPos", camera->getEntity()->getComponent<Transform>()->getPosition());
					shader->setUniform("directionalLight_direction", directionalLight->getDirection());
					shader->setUniform("directionalLight_ambientColor", directionalLight->getAmbientColor());
					shader->setUniform("directionalLight_color", directionalLight->getColor());

					shader->setUniform("time", (float)Time::get_time());

					shader->setUniform("isTwoSided", (int)material->isTwoSided());

					// Finally drawing..
					rendererCommands->bindMesh(mesh);

					// Update per instance transformations buffer only once..
					if (!batch.second->instancedDataHandled)
					{
						// Transformation matrices
						VertexBuffer<float>* instancedMatrixBuffer = mesh->getVertexBuffers()[1];
						size_t transformationsBuffSize = sizeof(float) * 16 * meshInstanceCount;
						instancedMatrixBuffer->update(0, transformationsBuffSize, batch.second->transformations);

						// Wind properties
						VertexBuffer<float>* instancedWindBuffer = mesh->getVertexBuffers()[2];
						size_t windPropertyBuffSize = sizeof(float) * meshInstanceCount;
						instancedWindBuffer->update(0, windPropertyBuffSize, batch.second->windProperties);

						delete[] batch.second->transformations;
						delete[] batch.second->windProperties;

						batch.second->instancedDataHandled = true;
					}

					rendererCommands->drawIndices_instanced(mesh);
					rendererCommands->unbindMesh(mesh);

					rendererCommands->unbindMaterial(material);

				}
			}

			void NatureRenderer::clear()
			{
			}


			void NatureRenderer::submit(Entity* entity)
			{
				std::shared_ptr<Mesh> mesh = entity->getComponent<Mesh>();
				std::shared_ptr<Material> material = entity->getComponent<Material>();

				bool createNewMeshCpy = false;
				for (std::pair<std::shared_ptr<Mesh>, std::shared_ptr<BatchData>>& batch : _batches)
				{
					if (batch.first == mesh)
					{
						if (batch.second->instanceCount < batch.first->getInstanceCount())
						{
							addToBatch(entity, *batch.second);
							return;
						}
						else
						{
							createNewMeshCpy = true;
						}
					}
				}
				
				createNewBatch(entity, material, mesh, createNewMeshCpy);
			}

			
			void NatureRenderer::createNewBatch(entities::Entity* entity, std::shared_ptr<Material>& material, std::shared_ptr<Mesh>& mesh, bool createMeshCpy)
			{
				printf("Creating new batch!\n");

				std::shared_ptr<BatchData> newBatch = std::make_shared<BatchData>(material, mesh->getInstanceCount());
				
				if (createMeshCpy)
				{
					newBatch->mesh = Mesh::create_mesh(
						mesh->getVertexBuffers(),
						mesh->getIndexBuffer(),
						DrawType::Triangles,
						mesh->getInstanceCount(),
						"",
						true
					);
				}
				else
				{
					newBatch->mesh = mesh;
				}
				
				// Create the per instance transformations buffer (Matrix4)
				std::shared_ptr<Transform> transform = entity->getComponent<Transform>();
				transform->update();
				
				addToTransformsBuff(*newBatch.get(), transform->getTransformationMatrix());

				// Create the actual vertex buffer for that..
				size_t transformationsBuffLength = 16 * newBatch->mesh->getInstanceCount();
				VertexBufferLayout transformationsBuffLayout({
					{ 4, ShaderDataType::Float4, true },
					{ 5, ShaderDataType::Float4, true },
					{ 6, ShaderDataType::Float4, true },
					{ 7, ShaderDataType::Float4, true }
				});
				VertexBuffer<float>* perInstanceTransformationsBuffer = VertexBuffer<float>::create_vertex_buffer(
					newBatch->transformations, 
					sizeof(float) * transformationsBuffLength, 
					BufferUsage::StaticDraw, 
					transformationsBuffLayout
				);

				// Create per instance "wind properties buffer"
				size_t windPropertiesBuffLength = newBatch->mesh->getInstanceCount();
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

				addToWindInitValsBuff(*newBatch);

				newBatch->instanceCount++;
				_batches.push_back(std::make_pair(mesh, newBatch));
			}

			void NatureRenderer::addToBatch(entities::Entity* entity, BatchData& batch)
			{
				std::shared_ptr<Transform> transform = entity->getComponent<Transform>();
				transform->update();
				mml::Matrix4 transformationMatrix = transform->getTransformationMatrix();
				
				addToTransformsBuff(batch, transform->getTransformationMatrix());
				addToWindInitValsBuff(batch);

				batch.instanceCount++;
			}

			// *May be wrong!?
			void NatureRenderer::addToTransformsBuff(BatchData& batch, const mml::Matrix4& transformationMatrix)
			{
				unsigned int& ptr = batch.staticArrDataPtr_transformations;
				if (ptr < batch.mesh->getInstanceCount() * 16)
				{
					for (int i = 0; i < 16; ++i)
						batch.transformations[ptr + i] = transformationMatrix[i];
					
					ptr += 16;
				}
			}
			void NatureRenderer::addToWindInitValsBuff(BatchData& batch)
			{
				unsigned int& ptr = batch.staticArrDataPtr_wind;
				if (ptr < batch.mesh->getInstanceCount())
				{
					batch.windProperties[ptr] = (float)(std::rand() % 100) * 0.1f;
					ptr++;
				}
			}

			void NatureRenderer::renderToShadowmap()
			{}
			
			void NatureRenderer::renderToScreen()
			{}

			const size_t NatureRenderer::getSize() const
			{
				return sizeof(*this);
			}
		}
	}
}