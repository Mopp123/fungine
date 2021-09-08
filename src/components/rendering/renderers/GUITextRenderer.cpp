
#include "GUITextRenderer.h"
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
			GUITextRenderer::GUITextRenderer()
			{
				// Create common guiText shader
				_shader = ShaderProgram::create_shader_program(
					"TextShader",
					ShaderStage::create_shader_stage("res/shaders/guiShaders/TextVertexShader.shader", ShaderStageType::VertexShader),
					ShaderStage::create_shader_stage("res/shaders/guiShaders/TextFragmentShader.shader", ShaderStageType::PixelShader)
				);

				// Create projection matrix for all text rendering
				const int windowWidth = core::Window::get_width();
				const int windowHeight = core::Window::get_height();
				const int windowAspectRatio = windowWidth / windowHeight;
				mml::create_orthographic_projection_matrix(_projectionMatrix, 0, windowWidth, 0, windowHeight, 0.0f, 1000.0f);

				// Create mesh used for all text character rendering
				_meshContainer = commonEntityFactory::create_entity__Plane2D({ 0, 0 }, { 1, 1 }, TEXT_RENDERING__MAX_CHAR_INSTANCE_COUNT);
				_mesh = _meshContainer->getComponent<Mesh>();

				// Create per instance vertex buffers

				// "transforms buffer" holds position and scale of every character of every text in the scene
				const unsigned int transformsBuffLength = TEXT_RENDERING__MAX_CHAR_INSTANCE_COUNT * TEXT_RENDERING__PER_INSTANCE_CHAR_FLOATS;
				float* emptyTransformsBuff = new float[transformsBuffLength];
				memset(emptyTransformsBuff, 0, sizeof(float) * transformsBuffLength);
				VertexBufferLayout transformsBuffLayout(
					{
						{ 1, ShaderDataType::Float4, true }, // transform (x,y = position | z,w = scale)
						{ 2, ShaderDataType::Float2, true }	 // texture offset
					}
				);
				VertexBuffer<float>* transformsBuffer = VertexBuffer<float>::create_vertex_buffer(
					emptyTransformsBuff,
					sizeof(float) * transformsBuffLength,
					BufferUsage::StreamDraw,
					transformsBuffLayout
				);

				delete[] emptyTransformsBuff;
				emptyTransformsBuff = nullptr;

				RendererCommands* rc = Graphics::get_renderer_commands();
				rc->bindMesh(_mesh.get());
				_mesh->addVertexBuffer(transformsBuffer);
				rc->unbindMesh(_mesh.get());

				// Get handle to that buffer
				_perInstanceBuffer_charTransforms = transformsBuffer;
			}
			GUITextRenderer::~GUITextRenderer()
			{
				// destroy the "gui mesh"
				delete _meshContainer;
			}

			void GUITextRenderer::update()
			{
				_totalLetterCount = 0;

				_batches.clear();
				for (Entity* entity : _entities)
					submit(entity);
			}

			void GUITextRenderer::submit(Entity* entity)
			{
				const std::shared_ptr<Transform> transform = entity->getComponent<Transform>();
				const mml::Vector3& position = transform->getPosition();
				const std::shared_ptr<GUIText> guiText = entity->getComponent<GUIText>();
				Font* font = guiText->getFont();
#ifdef DEBUG__MODE_FULL
				if (guiText == nullptr)
				{
					Debug::log(
						"Location: void GUITextRenderer::submit(Entity* entity)\n"
						"Attempted to submit entity for text rendering, but the entity's 'GUIText' component was nullptr!",
						DEBUG__ERROR_LEVEL__ERROR
					);
					return;
				}
#endif
				BatchData_TextRendering* batch = nullptr; // ..quite dangerous.. but fast..
				if (_batches.find(font) != _batches.end())
				{
					batch = _batches[font].get();
				}
				else
				{
					std::shared_ptr<BatchData_TextRendering> newBatch = std::make_shared<BatchData_TextRendering>();
					_batches.insert(std::make_pair(font, newBatch));
					batch = newBatch.get();
				}
				
				// update characters' transforms buffer
				
				// For each character in submitted text...
				std::unordered_map<char, FontGlyphData>& fontGlyphMapping = font->getGlyphMapping();
				int fontAtlasTileWidth = font->getTilePixelWidth();
				int charCount = (int)guiText->getText().size();
				float cursorPos = 0.0f;
				const float scalingFactor = guiText->getScalingFactor();

				size_t buffLength = sizeof(float) * charCount * TEXT_RENDERING__PER_INSTANCE_CHAR_FLOATS;

				float y = position.y;
				
				unsigned int& ptr = batch->currentDataPtr_properties;
				for (const char c : guiText->getText())
				{
					if (c != '\n')
					{
						const FontGlyphData& gd = fontGlyphMapping[c];
						if (c != ' ')
						{
							float posX = (position.x + cursorPos) + (gd.bearing.x * scalingFactor);
							float posY = y + (fontAtlasTileWidth - gd.bearing.y) * scalingFactor;

							float charWidth = fontAtlasTileWidth * scalingFactor;
							float charHeight = fontAtlasTileWidth * scalingFactor;

							batch->dataBuff_properties[ptr] = posX;
							batch->dataBuff_properties[ptr + 1] = posY;

							batch->dataBuff_properties[ptr + 2] = charWidth;
							batch->dataBuff_properties[ptr + 3] = charHeight;

							const mml::IVector2& textureOffset = gd.textureOffset;
							batch->dataBuff_properties[ptr + 4] = textureOffset.x;
							batch->dataBuff_properties[ptr + 5] = textureOffset.y;


							//_totalLetterCount++;
							batch->instanceCount++;
							ptr += TEXT_RENDERING__PER_INSTANCE_CHAR_FLOATS;
						}
						cursorPos += (gd.advance >> 6) * scalingFactor;
					}
					else
					{
						cursorPos = 0;
						y += fontAtlasTileWidth * scalingFactor;
					}
				}
				
				//_perInstanceBuffer_charTransforms->update(prevSize, buffLength, data);
				

			}

			void GUITextRenderer::render(
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

				for (const std::pair<Font*, std::shared_ptr<BatchData_TextRendering>>& batch : _batches)
				{
					const Font * const font = batch.first;
					const TextureAtlas * const textureAtlas = font->getTextureAtlas();
					const Texture * const texture = textureAtlas->getTexture();
					_shader->setUniform("textureRowCount", (float)textureAtlas->getTileCount());
					_shader->setUniform("fontColor", font->getColor());

					const unsigned int& instanceCount = batch.second->instanceCount;
					size_t buffLength = sizeof(float) * TEXT_RENDERING__PER_INSTANCE_CHAR_FLOATS * instanceCount;
					_perInstanceBuffer_charTransforms->update(0, buffLength, batch.second->dataBuff_properties);

					rc->bindTexture(texture, 0);

					// Draw the batch..
					rc->drawIndices_instanced(_mesh.get(), instanceCount);

					rc->unbindTexture(texture, 0);
				}
				rc->unbindShader();
				rc->unbindMesh(_mesh.get());
			}

			void GUITextRenderer::clear()
			{}

			const size_t GUITextRenderer::getSize() const
			{
				return sizeof(*this);
			}
		}
	}
}