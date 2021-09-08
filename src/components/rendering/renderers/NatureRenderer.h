#pragma once

#include "Renderer.h"
#include "components/rendering/Camera.h"
#include "components/rendering/lighting/Lights.h"
#include <vector>

namespace fungine
{
	namespace components
	{
		namespace rendering
		{
			struct BatchData_NatureRendering
			{
				std::shared_ptr<Mesh> mesh;
				std::shared_ptr<Material> material;
				float* transformations = nullptr;
				bool transformationsBuffUpdated = false;

				unsigned int staticArrDataPtr_transformations = 0;

				float* windProperties = nullptr;
				unsigned int staticArrDataPtr_wind = 0;				

				unsigned int instanceCount = 0;
				bool instancedDataHandled = false;

				BatchData_NatureRendering(std::shared_ptr<Material>& batchMaterial, size_t maxInstanceCount)
				{
					material = batchMaterial;
					transformations = new float[16 * maxInstanceCount];
					windProperties = new float[maxInstanceCount];
				}
				~BatchData_NatureRendering()
				{
					delete[] transformations;
					delete[] windProperties;

					printf("Batch destroyed!\n");
				}
			};

			class NatureRenderer : public Renderer
			{
			private:
				std::vector<std::pair<std::shared_ptr<Mesh>, std::shared_ptr<BatchData_NatureRendering>>> _batches;
				//bool _lockSubmitting = false;

			public:
				NatureRenderer();
				~NatureRenderer();

				virtual void update() override;

				virtual void submit(entities::Entity* entity) override;

				virtual void render(
					const mml::Matrix4& projectionMatrix,
					const mml::Matrix4& viewMatrix,
					unsigned int renderFlags
				) override;

				virtual void renderShadows(
				) override;


				virtual void clear() override;

				virtual const size_t getSize() const override;

			private:

				std::shared_ptr<BatchData_NatureRendering>& createNewBatch(std::shared_ptr<Material>& material, std::shared_ptr<Mesh>& mesh);
				void addToBatch(entities::Entity* entity, BatchData_NatureRendering& batch);
			
				void addToTransformsBuff(BatchData_NatureRendering& batch, const mml::Matrix4& transformationMatrix);
				void addToWindInitValsBuff(BatchData_NatureRendering& batch);
			};
		}
	}
}