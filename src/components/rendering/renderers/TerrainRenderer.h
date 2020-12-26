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

				std::vector<std::pair<std::shared_ptr<Material>, std::vector<entities::Entity*>>> _batches;

			public:

				TerrainRenderer(bool renderShadows = false);
				~TerrainRenderer();

				virtual void flush(
					const mml::Matrix4& projectionMatrix,
					const mml::Matrix4& viewMatrix,
					unsigned int renderFlags
				) override;

				virtual void clear() override;

				virtual const size_t getSize() const override;

			protected:

				virtual void submit(entities::Entity* e) override;

				virtual void setMaterialUniforms(
					const graphics::RendererCommands* rendererCommands,
					Material* material,
					graphics::ShaderProgram* shader
				) const override;

				virtual void setLightingUniforms(
					graphics::ShaderProgram* shader,
					const DirectionalLight* directionalLight
				) const override;

				virtual void setShadowUniforms(
					const graphics::RendererCommands* rendererCommands,
					graphics::ShaderProgram* shader,
					ShadowCaster& shadowCaster
				) const override;
			};
		}
	}
}