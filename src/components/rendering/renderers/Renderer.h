#pragma once

#include "components/Component.h"
#include "graphics/Graphics.h"
#include "entities/Entity.h"
#include "components/rendering/Mesh.h"
#include "components/rendering/Material.h"
#include "graphics/Framebuffer.h"
#include <unordered_map>

#define RENDERER_DEFAULT_NAME "Renderer"

namespace fungine
{
	namespace components
	{
		namespace rendering
		{
			class RenderQue
			{
			private:
				// key = material's component id, value = data required to render this batch
				std::unordered_map<std::shared_ptr<Material>, std::vector<std::shared_ptr<Mesh>>> _batches;

			public:

				void add(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);
				void clear();

				inline std::unordered_map<std::shared_ptr<Material>, std::vector<std::shared_ptr<Mesh>>>& getBatches() { return _batches; }

			};


			// What to render and to what framebuffer?
			struct RenderPass
			{
				graphics::Framebuffer* target = nullptr;
				RenderQue renderQue;
			};

			class Renderer : public Component
			{
			protected:

				static std::vector<Renderer*> s_allRenderers;

				std::vector<entities::Entity*> _entities; // Entities this renderer has been attached to
				
				bool _batchCreated = false;

				static graphics::Framebuffer* s_framebuffer; // *->TEMP : just testing framebuffer here..

			public:
				Renderer(const std::string& name = RENDERER_DEFAULT_NAME);
				virtual ~Renderer();
				
				virtual void onAttackToEntity(entities::Entity* entity) override;
				virtual void update() override;

				virtual void flush(
					const mml::Matrix4& projectionMatrix,
					const mml::Matrix4& viewMatrix
				) {}

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