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

			
			/* 
				Per instance data of Entity using "Instanced renderer" component
			
					* This is used to be able to modify Entity's Mesh's batch's instanced buffers
					at the position of this Entity's Mesh's position.

					* If Entity has a mesh which is using renderer that uses instancing,
					the renderer adds this component to the entity automatically.
			*/
			class BatchInstanceData : public Component
			{
			public:
				unsigned int position = 0;
				unsigned int batchIndex = 0;
				unsigned int buffElemLength = 1;
				Mesh* mesh = nullptr;
				float** buffer;
				float* bufferToTrack;

				// This is true until second update (if we want to have some special stuff on first update)
				bool init = true;

				BatchInstanceData(unsigned int position, unsigned int batchIndex, float** buff, float* buffToTrack, unsigned int buffElemLength);
				~BatchInstanceData();

				virtual void update() override;

				virtual const size_t getSize() const override
				{
					return sizeof(*this);
				}
			};

			class Renderer : public Component
			{
			protected:

				
				std::vector<entities::Entity*> _entities; // Entities this renderer has been attached to
				bool _entitiesChanged = false; // True, if new entities added this renderer as component or an entity with this component gets destroyed
				
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
				inline bool entitiesChanged() const { return _entitiesChanged; }
				virtual const size_t getSize() const override;

			protected:
				virtual void submit(entities::Entity* entity) {}
			};
		}
	}
}