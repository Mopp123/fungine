
#include "GUIRenderer.h"
#include "graphics/shaders/ShaderStage.h"
#include "components/guiComponents/GUIImage.h"
#include "entities/commonEntitiesLib/shapes3D/CommonShapes.h"

#include "core/Debug.h"


namespace fungine
{
	using namespace graphics;
	using namespace entities;

	namespace components
	{
		namespace rendering
		{
			GUIRenderer::GUIRenderer()
			{
				// Create common gui shader
				_shader = ShaderProgram::create_shader_program(
					"GuiShader", 
					ShaderStage::create_shader_stage("res/shaders/guiShaders/GUIVertexShader.shader", ShaderStageType::VertexShader),
					ShaderStage::create_shader_stage("res/shaders/guiShaders/GUIFragmentShader.shader", ShaderStageType::PixelShader)
				);

				// Create projection matrix for all gui rendering
				mml::create_orthographic_projection_matrix(_projectionMatrix, -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 1000.0f);

				// Create mesh used for all gui rendering
				_meshContainer = commonEntityFactory::create_entity__Plane({ 0, 0, 0 }, { {0,1,0}, 0 }, { 1, 1, 1 });
				_mesh = _meshContainer->getComponent<Mesh>();

				// Create per instance vertex buffer
				const unsigned int instancedBuffLength = GUI_RENDERING__MAX_INSTANCE_COUNT * GUI_RENDERING__INSTACED_BUFF_FLOAT_COUNT;
				float* emptyInstancedBuff = new float[instancedBuffLength];
				memset(emptyInstancedBuff, 0, sizeof(float) * instancedBuffLength);
				VertexBufferLayout instancedBuffLayout(
					{
						{ 1, ShaderDataType::Float3, true }, // position
						{ 2, ShaderDataType::Float2, true }, // scale
						{ 3, ShaderDataType::Float2, true } // texture offset
					}
				);
				VertexBuffer<float>* instancedBuffer = VertexBuffer<float>::create_vertex_buffer(
					emptyInstancedBuff,
					sizeof(float) * instancedBuffLength,
					BufferUsage::StreamDraw,
					instancedBuffLayout
				);

				delete[] emptyInstancedBuff;
				emptyInstancedBuff = nullptr;

				RendererCommands* rc = Graphics::get_renderer_commands();
				rc->bindMesh(_mesh.get());
				_mesh->addVertexBuffer(instancedBuffer);
				rc->unbindMesh(_mesh.get());

				// Get handle to that buffer
				const std::vector<VertexBuffer<float>*>& buffers = _mesh->getVertexBuffers();
				_perInstanceBuffer = buffers[buffers.size() - 1];
			}
			GUIRenderer::~GUIRenderer()
			{
				// destroy the "gui mesh"

				delete _meshContainer;
			}

			void GUIRenderer::update()
			{
				// submit (which creates all batches, etc) all entities only if the "_entities" list has changed
				bool entitiesChanged = _prevEntityList.empty();
				if(!entitiesChanged) entitiesChanged = memcmp(&_entities[0], &_prevEntityList[0], sizeof(Entity) * _entities.size()) == 0;

				if (!entitiesChanged)
				{
					// Update all data in batches according to current state of the entities inside the batch
					for (auto& b : _batches)
						updateBatch(b.second);
				}
				else
				{
					// Submit all entities, create batches, etc..
					_batches.clear();
					_prevEntityList.clear();
					for (Entity* entity : _entities)
						submit(entity);
				}
			}

			void GUIRenderer::submit(Entity* entity)
			{
				const std::shared_ptr<GUIImage> guiImg = entity->getComponent<GUIImage>();
#ifdef DEBUG__MODE_FULL
				if (guiImg == nullptr)
				{
					Debug::log(
						"Location: void GUIRenderer::submit(Entity* entity)\n"
						"Attempted to submit entity for gui rendering, but the entity's 'GUIImage' component was nullptr!",
						DEBUG__ERROR_LEVEL__ERROR
					);
					return;
				}
#endif
				const Texture* texture = guiImg->getTexture();
#ifdef DEBUG__MODE_FULL
				if (texture == nullptr)
				{
					Debug::log(
						"Location: void GUIRenderer::submit(Entity* entity)\n"
						"Attempted to submit entity for gui rendering, but the entity's GUIImage component's texture was nullptr!",
						DEBUG__ERROR_LEVEL__ERROR
					);
					return;
				}
#endif
				// Attempt to find existing batch for this guiImage
				if (_batches.find(texture) != _batches.end())
				{
#ifdef DEBUG__MODE_FULL
					if (_batches[texture] == nullptr)
					{
						Debug::log(
							"Location: void GUIRenderer::submit(Entity* entity)\n"
							"Attempted to add entity to an existing batch, but the batch's 'BatchData' was nullptr!",
							DEBUG__ERROR_LEVEL__ERROR
						);
						return;
					}
#endif
					if(_batches[texture]->instanceCount < GUI_RENDERING__MAX_INSTANCE_COUNT)
						addToBatch(entity, _batches[texture]);
					else
						createNewBatch(texture, entity);
				}
				else // If no suitable batch was found -> create new batch
				{
					createNewBatch(texture, entity);
				}
				_prevEntityList.push_back(entity);
			}

			void GUIRenderer::render(
				const mml::Matrix4& projectionMatrix,
				const mml::Matrix4& viewMatrix,
				unsigned int renderFlags
			)
			{
				RendererCommands* rc = Graphics::get_renderer_commands();
				
				rc->bindMesh(_mesh.get());
				
				rc->bindShader(_shader);
				// Common uniforms for all gui things..
				_shader->setUniform("projectionMatrix", _projectionMatrix);

				for (const std::pair<const Texture* const, std::shared_ptr<BatchData_GUIRendering>>& batch : _batches)
				{
					const Texture* const batchTexture = batch.first;
					const std::shared_ptr<BatchData_GUIRendering>& batchData = batch.second;
					const float* instancedBuffer = batchData->dataBuff;
					
					rc->bindTexture(batchTexture, 0);

					// Batch specific uniforms
					_shader->setUniform("texRowCount", batchData->texAtlasRowCount);
					
					// Update all per instance data for the per instance vertex buffer
					_perInstanceBuffer->update(0, sizeof(float) * 7 * batchData->instanceCount, instancedBuffer);

					// Draw the batch..
					rc->drawIndices_instanced(_mesh.get(), batch.second->instanceCount);
				
					rc->unbindTexture(batchTexture, 0);
				}

				rc->unbindShader();
				rc->unbindMesh(_mesh.get());
			}

			void GUIRenderer::clear()
			{
			}

			void GUIRenderer::createNewBatch(const Texture* const texture, Entity* entity)
			{
				std::shared_ptr<BatchData_GUIRendering> batch = std::make_shared<BatchData_GUIRendering>();
				addToBatch(entity, batch);
				_batches.insert(std::make_pair(texture, batch));
			}

			void GUIRenderer::addToBatch(entities::Entity* entity, std::shared_ptr<BatchData_GUIRendering>& batch)
			{
				batch->entities.push_back(entity);
				batch->instanceCount++;
			}

			void GUIRenderer::updateBatch(
				std::shared_ptr<BatchData_GUIRendering>& batch
			)
			{
				float* buff = batch->dataBuff;
				unsigned int& ptr = batch->currentDataPtr;
				ptr = 0;

				for (Entity* e : batch->entities)
				{
					const mml::Vector2& textureOffset = e->getComponent<GUIImage>()->getTextureOffset();
					std::shared_ptr<Transform> t = e->getComponent<Transform>();
					const mml::Vector3 pos = t->getPosition();
					const mml::Vector3 scale = t->getScale();

					// pos
					buff[ptr] = pos.x;
					buff[ptr + 1] = pos.y;
					buff[ptr + 2] = pos.z;
					// scale
					buff[ptr + 3] = scale.x;
					buff[ptr + 4] = scale.y;
					// texture offset
					buff[ptr + 5] = textureOffset.x;
					buff[ptr + 6] = textureOffset.y;

					ptr += 7;
				}
			}

			const size_t GUIRenderer::getSize() const
			{
				return sizeof(*this);
			}
		}
	}
}