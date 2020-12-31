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

				static std::vector<Renderer*> s_allRenderers;
				static graphics::Framebuffer* s_framebuffer; // *->TEMP : just testing framebuffer here..

			public:
				Renderer(const std::string& name = RENDERER_DEFAULT_NAME);
				virtual ~Renderer();
				
				virtual void onAttackToEntity(entities::Entity* entity) override;
				virtual void update() override;

				virtual void render(
					const mml::Matrix4& projectionMatrix,
					const mml::Matrix4& viewMatrix,
					unsigned int renderFlags
				) {}

				virtual void renderShadows() {}

				virtual void clear() {}

				inline static std::vector<Renderer*> get_all_renderers() { return s_allRenderers; }
				inline static graphics::Framebuffer* get_screen_framebuffer() { return s_framebuffer; }
				virtual const size_t getSize() const override;

			protected:
				virtual void submit(entities::Entity* entity) {}
			};
		}
	}
}