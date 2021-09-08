
#include "GUIRenderer.h"
#include "core/window/Window.h"
#include "graphics/shaders/ShaderStage.h"
#include "entities/commonEntitiesLib/shapes2D/CommonShapes2D.h"

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
				const int windowWidth = core::Window::get_width();
				const int windowHeight = core::Window::get_height();
				const int windowAspectRatio = windowWidth / windowHeight;
				mml::create_orthographic_projection_matrix(_projectionMatrix, 0, windowWidth, 0, windowHeight, 0.0f, 1000.0f);

				// Create mesh used for all gui rendering
				_meshContainer = commonEntityFactory::create_entity__Plane2D({ 0, 0 }, { 1, 1 }, GUI_RENDERING__MAX_INSTANCE_COUNT);
				_mesh = _meshContainer->getComponent<Mesh>();
				
				// Create per instance vertex buffers
				const unsigned int transformsBuffLength = GUI_RENDERING__MAX_INSTANCE_COUNT * 16;
				const unsigned int imgDatBuffLength = GUI_RENDERING__MAX_INSTANCE_COUNT * GUIIMAGE__BUFFER_LENGTH__graphicalData;
				float* emptyTransformsBuff = new float[transformsBuffLength];
				float* emptyImgDatBuff = new float[imgDatBuffLength];
				memset(emptyTransformsBuff, 0, sizeof(float) * transformsBuffLength);
				memset(emptyImgDatBuff, 0, sizeof(float) * imgDatBuffLength);
				VertexBufferLayout transformsBuffLayout(
					{
						{ 1, ShaderDataType::Float4, true }, // transformation matrix row 0
						{ 2, ShaderDataType::Float4, true }, // transformation matrix row 1
						{ 3, ShaderDataType::Float4, true }, // transformation matrix row 2
						{ 4, ShaderDataType::Float4, true }	 // transformation matrix row 3
					}
				);
				VertexBufferLayout imgDatBuffLayout(
					{
						{ 5, ShaderDataType::Float4, true }, // color
						{ 6, ShaderDataType::Float4, true }, // border color
						{ 7, ShaderDataType::Float, true },  // border thickness
						{ 8, ShaderDataType::Float2, true }	 // texture offset
					}
				);
				VertexBuffer<float>* transformsBuffer = VertexBuffer<float>::create_vertex_buffer(
					emptyTransformsBuff,
					sizeof(float) * transformsBuffLength,
					BufferUsage::StreamDraw,
					transformsBuffLayout
				);
				VertexBuffer<float>* imgDatBuffer = VertexBuffer<float>::create_vertex_buffer(
					emptyImgDatBuff,
					sizeof(float) * imgDatBuffLength,
					BufferUsage::StreamDraw,
					imgDatBuffLayout
				);

				delete[] emptyTransformsBuff;
				delete[] emptyImgDatBuff;
				emptyTransformsBuff = nullptr;
				emptyImgDatBuff = nullptr;

				RendererCommands* rc = Graphics::get_renderer_commands();
				rc->bindMesh(_mesh.get());
				_mesh->addVertexBuffer(transformsBuffer);
				_mesh->addVertexBuffer(imgDatBuffer);
				rc->unbindMesh(_mesh.get());

				// Get handle to that buffer
				_perInstanceBuffer_transforms = transformsBuffer;
				_perInstanceBuffer_imgProperties = imgDatBuffer;

			}
			GUIRenderer::~GUIRenderer()
			{
				// destroy the "gui mesh"
				delete _meshContainer;
			}

			void GUIRenderer::update()
			{
				if(_entitiesChanged)
				{
					// Submit all entities, create batches, etc..
					_batches.clear();
					for (Entity* entity : _entities)
						submit(entity);

					_entitiesChanged = false;
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
				const TextureAtlas* texture = guiImg->getTextureAtlas();
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
						addToBatch(entity, _batches[texture].get());
					else
						Debug::log(
							"Location: void GUIRenderer::submit(Entity* entity)\n"
							"Attempted to submit for GUIRendering, but maximum batch size was reached for this guiImage",
							DEBUG__ERROR_LEVEL__ERROR
						);
				}
				else // If no suitable batch was found -> create new batch
				{
					BatchData_GUIRendering* newBatch = createNewBatch(texture);
					addToBatch(entity, newBatch);
				}
			}

			void GUIRenderer::render(
				const mml::Matrix4& projectionMatrix,
				const mml::Matrix4& viewMatrix,
				unsigned int renderFlags
			)
			{
				RendererCommands* rc = Graphics::get_renderer_commands();
				rc->cullFace(CullFace::None);

				rc->bindMesh(_mesh.get());
				
				rc->bindShader(_shader);
				// Common uniforms for all gui things..
				_shader->setUniform("projectionMatrix", _projectionMatrix);

				for (const std::pair<const TextureAtlas* const, std::shared_ptr<BatchData_GUIRendering>>& batch : _batches)
				{
					const Texture* const batchTexture = batch.first->getTexture();
					const std::shared_ptr<BatchData_GUIRendering>& batchData = batch.second;
					const float* transformsBuffer = batchData->dataBuff_transforms;
					const float* imgPropertiesBuffer = batchData->dataBuff_guiImgProperties;

					rc->bindTexture(batchTexture, 0);

					// Batch specific uniforms
					_shader->setUniform("textureRowCount", (float)batch.first->getTileCount());
					
					// Update all per instance data for the per instance vertex buffers
					_perInstanceBuffer_transforms->update(0, sizeof(float) * 16 * batchData->instanceCount, transformsBuffer);
					_perInstanceBuffer_imgProperties->update(0, sizeof(float) * GUIIMAGE__BUFFER_LENGTH__graphicalData * batchData->instanceCount, imgPropertiesBuffer);

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

			BatchData_GUIRendering* GUIRenderer::createNewBatch(const TextureAtlas* const texture)
			{
				std::shared_ptr<BatchData_GUIRendering> batch = std::make_shared<BatchData_GUIRendering>();
				_batches.insert(std::make_pair(texture, batch));
				return _batches[texture].get();
			}

			void GUIRenderer::addToBatch(entities::Entity* entity, BatchData_GUIRendering* batch)
			{
				std::shared_ptr<Transform> transform = entity->getComponent<Transform>();
				std::shared_ptr<GUIImage> guiImg = entity->getComponent<GUIImage>();

				float* transformsBuffToTrack = transform->getTransformationMatrixBuff();
				float* imgDatBuffToTrack = guiImg->getGraphicalDataBuffer();

				std::shared_ptr<BatchInstanceData> entityBatchData_transforms = std::make_shared<BatchInstanceData>(
					batch->instanceCount,
					_batches.size() - 1,
					&batch->dataBuff_transforms,
					transformsBuffToTrack,
					16
				);
				std::shared_ptr<BatchInstanceData> entityBatchData_guiImg = std::make_shared<BatchInstanceData>(
					batch->instanceCount,
					_batches.size() - 1,
					&batch->dataBuff_guiImgProperties,
					imgDatBuffToTrack,
					GUIIMAGE__BUFFER_LENGTH__graphicalData
				); 
				entity->addComponent(entityBatchData_transforms);
				entity->addComponent(entityBatchData_guiImg);

				transform->update();
				addToTransformsBuff(batch, transform->getTransformationMatrix());
				addToGUIImgPropertiesBuff(batch, guiImg->getTextureOffset(), guiImg->getColor(), guiImg->getBorderColor(), guiImg->getBorderThickness());
				batch->instanceCount++;
			}

			void GUIRenderer::addToTransformsBuff(BatchData_GUIRendering* batch, const mml::Matrix4& transformationMatrix)
			{
				unsigned int& ptr = batch->currentDataPtr_transforms;
				if (ptr < _mesh->getInstanceCount() * 16)
				{
					for (int i = 0; i < 16; ++i)
						batch->dataBuff_transforms[ptr + i] = transformationMatrix[i];

					ptr += 16;
				}
			}
			
			void GUIRenderer::addToGUIImgPropertiesBuff(
				BatchData_GUIRendering* batch, 
				const mml::Vector2& texOffset, 
				const mml::Vector4& color,
				const mml::Vector4& borderColor,
				const int& borderThickness
			)
			{
				unsigned int& ptr = batch->currentDataPtr_guiImgProperties;
				if (ptr < _mesh->getInstanceCount() * GUIIMAGE__BUFFER_LENGTH__graphicalData)
				{
					batch->dataBuff_guiImgProperties[ptr] = color.x;
					batch->dataBuff_guiImgProperties[ptr + 1] = color.y;
					batch->dataBuff_guiImgProperties[ptr + 2] = color.z;
					batch->dataBuff_guiImgProperties[ptr + 3] = color.w;

					batch->dataBuff_guiImgProperties[ptr + 4] = borderColor.x;
					batch->dataBuff_guiImgProperties[ptr + 5] = borderColor.y;
					batch->dataBuff_guiImgProperties[ptr + 6] = borderColor.z;
					batch->dataBuff_guiImgProperties[ptr + 7] = borderColor.w;

					batch->dataBuff_guiImgProperties[ptr + 8] = (float)borderThickness;


					batch->dataBuff_guiImgProperties[ptr + 9] = texOffset.x;
					batch->dataBuff_guiImgProperties[ptr + 10] = texOffset.y;

					ptr += GUIIMAGE__BUFFER_LENGTH__graphicalData;
				}
			}


			const size_t GUIRenderer::getSize() const
			{
				return sizeof(*this);
			}
		}
	}
}