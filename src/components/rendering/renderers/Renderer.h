#pragma once

#include "components/Component.h"
#include "graphics/Graphics.h"
#include "graphics/RendererCommands.h"
#include "entities/Entity.h"
#include "components/rendering/Mesh.h"
#include "components/rendering/Material.h"
#include "components/rendering/lighting/Lights.h"
#include "graphics/Framebuffer.h"
#include <unordered_map>

#define RENDERER_DEFAULT_NAME "Renderer"

namespace fungine
{
	namespace components
	{
		namespace rendering
		{
			enum RenderFlags
			{
				RenderNone =		0x0,
				RenderGeometry =	0x1,
				RenderMaterial =	0x2,
				RenderLighting =	0x4,
				RenderShadows =		0x8
			};

			class Renderer : public Component
			{
			protected:

				
				std::vector<entities::Entity*> _entities; // Entities this renderer has been attached to
				
				bool _batchCreated = false;
				bool _renderShadows = false;

				static std::vector<Renderer*> s_allRenderers;
				static graphics::Framebuffer* s_framebuffer; // *->TEMP : just testing framebuffer here..

			public:
				Renderer(bool renderShadows, const std::string& name = RENDERER_DEFAULT_NAME);
				virtual ~Renderer();
				
				virtual void onAttackToEntity(entities::Entity* entity) override;
				virtual void update() override;

				virtual void flush(
					const mml::Matrix4& projectionMatrix,
					const mml::Matrix4& viewMatrix,
					unsigned int renderFlags
				) {}

				virtual void clear() {}

				inline bool rendersShadows() const { return _renderShadows; }

				inline static std::vector<Renderer*> get_all_renderers() { return s_allRenderers; }
				inline static graphics::Framebuffer* get_screen_framebuffer() { return s_framebuffer; }
				virtual const size_t getSize() const override;

			protected:
				virtual void submit(entities::Entity* entity) {}

				virtual void setMaterialUniforms(
					const graphics::RendererCommands* rendererCommands, 
					Material* material,
					graphics::ShaderProgram* shader
				) const {}
				
				virtual void setLightingUniforms(
					graphics::ShaderProgram* shader, 
					const DirectionalLight* directionalLight
				) const {}
				
				virtual void setShadowUniforms(
					const graphics::RendererCommands* rendererCommands,
					graphics::ShaderProgram* shader,
					ShadowCaster& shadowCaster
				) const {}
			};
		}
	}
}