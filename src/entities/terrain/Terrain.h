#pragma once

#include "entities/Entity.h"
#include "components/common/Transform.h"
#include "components/rendering/Mesh.h"
#include "components/rendering/Material.h"
#include "components/rendering/renderers/TerrainRenderer.h"

namespace fungine
{
	namespace entities
	{
		class Terrain : public Entity
		{
		private:

			std::shared_ptr<components::Transform> _transform = nullptr;

			std::shared_ptr<components::Mesh> _mesh = nullptr;
			std::shared_ptr<components::Material> _material = nullptr;

			float _tileWidth = 5.0f;
			unsigned int _verticesPerRow = 512;

			float _heightModifier = 128.0f;
			std::vector<float> _heightmap;

		public:

			Terrain(float tileWidth,
				ImageData* heightmapImage,
				graphics::Texture* blendmapTexture,
				graphics::Texture* black_diffuse,	graphics::Texture* black_specular,	graphics::Texture* black_normal,
				graphics::Texture* red_diffuse,		graphics::Texture* red_specular,	graphics::Texture* red_normal,
				graphics::Texture* green_diffuse,	graphics::Texture* green_specular,	graphics::Texture* green_normal,
				graphics::Texture* blue_diffuse,	graphics::Texture* blue_specular,	graphics::Texture* blue_normal,
				const std::shared_ptr<components::rendering::Renderer>& rendererToUse,
				graphics::ShaderProgram* shaderToUse
			);

			~Terrain();


			float getHeightAt(const float& x, const float& z);

			inline float getTileWidth() const { return _tileWidth; }
			inline unsigned int getVerticesPerRow() const { return _verticesPerRow; }

		private:

			void generateMesh();
		};
	}
}