#pragma once

#include "components/Component.h"
#include "entities/Entity.h"
#include "components/rendering/Mesh.h"
#include "components/rendering/Material.h"
#include "graphics/Framebuffer.h"
#include <unordered_map>

/*
	Renderer's are completely static components
*/
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

				std::vector<entities::Entity*> _entities; // Entities this renderer has been attached to
				RenderPass _renderPass;

				static graphics::Framebuffer* s_framebuffer; // *->TEMP : just testing framebuffer here..

			public:
				Renderer();
				virtual ~Renderer();
				
				virtual void onAttackToEntity(entities::Entity* entity) override;
				virtual void update() override;

				// processes all render passes and after that clears all render ques
				virtual void flush();

				virtual const size_t getSize() const override;

			protected:
				virtual void submit(entities::Entity* entity);
				void clear();
			};
		}
	}
}