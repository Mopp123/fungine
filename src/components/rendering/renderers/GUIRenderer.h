#pragma once

#include "Renderer.h"
#include "components/rendering/Camera.h"
#include "components/rendering/lighting/Lights.h"
#include "graphics/shaders/ShaderProgram.h"
#include <vector>

#define GUI_RENDERING__MAX_INSTANCE_COUNT 100000
#define GUI_RENDERING__INSTACED_BUFF_FLOAT_COUNT 16 + 2

namespace fungine
{
	namespace components
	{
		namespace rendering
		{
			struct BatchData_GUIRendering
			{
				float* dataBuff = nullptr;
				float texAtlasRowCount = 1.0f;

				std::vector<entities::Entity*> entities;

				unsigned int currentDataPtr = 0;
				unsigned int instanceCount = 0;

				BatchData_GUIRendering()
				{
					// transformation matrix = 16 floats, texture offset = 2 floats: TOTAL = 16 + 2 = 18 floats
					dataBuff = new float[GUI_RENDERING__MAX_INSTANCE_COUNT * GUI_RENDERING__INSTACED_BUFF_FLOAT_COUNT];
				}
				~BatchData_GUIRendering()
				{
					delete[] dataBuff;
					dataBuff = nullptr;
				}
			};

			class GUIRenderer : public Renderer
			{
			private:

				graphics::ShaderProgram* _shader = nullptr; // common shader for all gui rendering
				mml::Matrix4 _projectionMatrix; // Orthographic proj mat for all gui rendering

				// List of entities from previous frame. This is used to track, if some entities were destroyed or some new entities added.
				// If this is different than the current "_entities" list, we "re submit" and recreate all batches
				std::vector<entities::Entity*> _prevEntityList;
				
				std::unordered_map<const graphics::Texture*, std::shared_ptr<BatchData_GUIRendering>> _batches;

				entities::Entity* _meshContainer = nullptr; // Single entity with "quad mesh" used for all gui rendering
				std::shared_ptr<Mesh> _mesh = nullptr; // Mesh of the "_meshContainer entity"
				graphics::VertexBuffer<float>* _perInstanceBuffer = nullptr; // Vertex buffer which holds all the per instance data


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
				void createNewBatch(const graphics::Texture* const texture, entities::Entity* entity);
				void addToBatch(entities::Entity* entity, std::shared_ptr<BatchData_GUIRendering>& batch);
			};
		}
	}
}