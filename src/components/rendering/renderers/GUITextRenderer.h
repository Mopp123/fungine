#pragma once

#include "Renderer.h"
#include "components/rendering/Camera.h"
#include "components/guiComponents/GUIText.h"
#include "graphics/shaders/ShaderProgram.h"
#include <vector>
#include <unordered_map>



#define TEXT_RENDERING__MAX_CHAR_INSTANCE_COUNT 110000
// pos = 2 floats scale = 2 floats
#define TEXT_RENDERING__PER_INSTANCE_CHAR_FLOATS 6

namespace fungine
{
	namespace components
	{
		namespace rendering
		{
			struct BatchData_TextRendering
			{
				float* dataBuff_properties = nullptr;

				unsigned int currentDataPtr_properties = 0;

				unsigned int instanceCount = 0;

				BatchData_TextRendering()
				{
					dataBuff_properties = new float[TEXT_RENDERING__MAX_CHAR_INSTANCE_COUNT * TEXT_RENDERING__PER_INSTANCE_CHAR_FLOATS];
				}
				~BatchData_TextRendering()
				{
					delete[] dataBuff_properties;
					dataBuff_properties = nullptr;
				}
			};

			

			class GUITextRenderer : public Renderer
			{
			private:

				graphics::ShaderProgram* _shader = nullptr; // common shader for all text rendering
				mml::Matrix4 _projectionMatrix; // Orthographic proj mat for all text rendering

				std::unordered_map<Font*, std::shared_ptr<BatchData_TextRendering>> _batches;
				
				entities::Entity* _meshContainer = nullptr; // Single entity with "quad mesh" used for all text rendering
				std::shared_ptr<Mesh> _mesh = nullptr; // Mesh of the "_meshContainer entity"
				graphics::VertexBuffer<float>* _perInstanceBuffer_charTransforms = nullptr;

				unsigned int _totalLetterCount = 0; // Combined total letter count of all currently rendered texts

			public:
				GUITextRenderer();
				~GUITextRenderer();

				virtual void update() override;

				virtual void submit(entities::Entity* entity) override;

				virtual void render(
					const mml::Matrix4& projectionMatrix,
					const mml::Matrix4& viewMatrix,
					unsigned int renderFlags
				) override;

				virtual void clear() override;

				virtual const size_t getSize() const override;

			};
		}
	}
}