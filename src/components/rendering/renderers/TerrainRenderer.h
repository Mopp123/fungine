#pragma once

#include "Renderer.h"
#include "components/rendering/Material.h"

namespace fungine
{
	namespace components
	{
		namespace rendering
		{
			

			class TerrainRenderer : public Renderer
			{
			private:

				std::vector<entities::Entity*> _renderList;

			public:

				TerrainRenderer();
				~TerrainRenderer();

				virtual void render(
					const mml::Matrix4& projectionMatrix,
					const mml::Matrix4& viewMatrix,
					unsigned int renderFlags
				) override;

				virtual void clear() override;

				virtual const size_t getSize() const override;

			protected:

				virtual void submit(entities::Entity* e) override;
			};
		}
	}
}