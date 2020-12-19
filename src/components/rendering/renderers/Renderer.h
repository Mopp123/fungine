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
			private:
				
				static std::vector<entities::Entity*> s_entities; // Entities this same renderer has been attached to
				static unsigned int s_processedEntities;
				static RenderPass s_renderPass;

				static bool s_isReadyToFlush;

				static graphics::Framebuffer* s_framebuffer; // *->TEMP : just testing framebuffer here..

			public:

				Renderer();
				virtual ~Renderer();

				virtual void onAttackToEntity(entities::Entity* entity) override;
				virtual void update() override;

				// processes all render passes and after that clears all render ques
				virtual void flush();

				virtual const size_t getSize() const override;

			private:
				virtual void submit(entities::Entity* entity);
				static void clear();
			};
		}
	}
}