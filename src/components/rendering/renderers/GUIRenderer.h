#pragma once

#include "Renderer.h"
#include "components/rendering/Camera.h"
#include "components/rendering/lighting/Lights.h"
#include "components/guiComponents/GUIImage.h"
#include "graphics/shaders/ShaderProgram.h"
#include <vector>

#define GUI_RENDERING__MAX_INSTANCE_COUNT 110000

namespace fungine
{
	namespace components
	{
		namespace rendering
		{
			struct BatchData_GUIRendering
			{
				float* dataBuff_transforms = nullptr;
				float* dataBuff_guiImgProperties = nullptr;
				
				unsigned int currentDataPtr_transforms = 0;
				unsigned int currentDataPtr_guiImgProperties = 0;
				unsigned int instanceCount = 0;

				BatchData_GUIRendering()
				{
					dataBuff_transforms = new float[GUI_RENDERING__MAX_INSTANCE_COUNT * 16];
					dataBuff_guiImgProperties = new float[GUI_RENDERING__MAX_INSTANCE_COUNT * GUIIMAGE__BUFFER_LENGTH__graphicalData];
				}
				~BatchData_GUIRendering()
				{
					delete[] dataBuff_transforms;
					delete[] dataBuff_guiImgProperties;
					dataBuff_transforms = nullptr;
					dataBuff_guiImgProperties = nullptr;
				}
			};

			class GUIRenderer : public Renderer
			{
			private:

				graphics::ShaderProgram* _shader = nullptr; // common shader for all gui rendering
				mml::Matrix4 _projectionMatrix; // Orthographic proj mat for all gui rendering

				std::unordered_map<const graphics::TextureAtlas*, std::shared_ptr<BatchData_GUIRendering>> _batches;

				entities::Entity* _meshContainer = nullptr; // Single entity with "quad mesh" used for all gui rendering
				std::shared_ptr<Mesh> _mesh = nullptr; // Mesh of the "_meshContainer entity"
				graphics::VertexBuffer<float>* _perInstanceBuffer_transforms = nullptr;
				graphics::VertexBuffer<float>* _perInstanceBuffer_imgProperties = nullptr;


			public:
				GUIRenderer();
				~GUIRenderer();

				virtual void update() override;

				virtual void submit(entities::Entity* entity) override;

				virtual void render(
					const mml::Matrix4& projectionMatrix,
					const mml::Matrix4& viewMatrix,
					unsigned int renderFlags
				) override;

				virtual void clear() override;

				virtual const size_t getSize() const override;

			private:
				
				// creates new batch and adds entity to it
				BatchData_GUIRendering* createNewBatch(const graphics::TextureAtlas* const texture);
				void addToBatch(entities::Entity* entity, BatchData_GUIRendering* batch);
				void addToTransformsBuff(BatchData_GUIRendering* batch, const mml::Matrix4& transformationMatrix);
				void addToGUIImgPropertiesBuff(
					BatchData_GUIRendering* batch,
					const mml::Vector2& texOffset,
					const mml::Vector4& color,
					const mml::Vector4& borderColor,
					const int& borderThickness
				);
			};
		}
	}
}