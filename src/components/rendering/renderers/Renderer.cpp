
#include "Renderer.h"
#include "core/window/Window.h"
#include "utils/Time.h"
#include "components/rendering/Camera.h"
#include "core/Debug.h"

namespace fungine
{
	using namespace entities;
	using namespace graphics;

	namespace components
	{
		namespace rendering
		{
			
			BatchInstanceData::BatchInstanceData(unsigned int position, unsigned int batchIndex, float** buff, float* buffToTrack, unsigned int buffElemLength) :
				Component("EntityBatchData", true), position(position), batchIndex(batchIndex), buffElemLength(buffElemLength)
			{
				buffer = buff;
				bufferToTrack = buffToTrack;
				update();
			}
			BatchInstanceData::~BatchInstanceData()
			{}


			void BatchInstanceData::update()
			{
				memcpy(*buffer + position * buffElemLength, &bufferToTrack[0], sizeof(float) * buffElemLength);
			}


			std::vector<Renderer*> Renderer::s_allRenderers;
			Framebuffer* Renderer::s_framebuffer = nullptr;

			Renderer::Renderer(const std::string& name) :
				Component(name, false, true)
			{
				if (!s_framebuffer)
					s_framebuffer = Framebuffer::create_framebuffer(core::Window::get_width(), core::Window::get_height(), true);

				s_allRenderers.push_back(this);
			}

			Renderer::~Renderer()
			{
				Debug::notify_on_destroy(_name + "(Renderer)");
				s_allRenderers.erase(std::find(s_allRenderers.begin(), s_allRenderers.end(), this));
			}

			// Removes entity from render list of this renderer
			// *This shouldn't be used too much especially continuously. Very slow operation!!!
			void Renderer::removeFromRenderList(entities::Entity* e)
			{
				for (int i = 0; i < _entities.size(); i++)
				{
					Entity* entity = _entities[i];

					std::vector<std::shared_ptr<BatchInstanceData>> batchInstanceDatas = entity->getComponents<BatchInstanceData>();
					for (std::shared_ptr<BatchInstanceData> b : batchInstanceDatas)
						if (b) entity->removeComponent(b);

					if (e == entity)
					{
						_entities.erase(_entities.begin() + i);
						_entitiesChanged = true;
					}
				}
			}

			void Renderer::onAttackToEntity(entities::Entity* entity)
			{
				add_to_dynamic_list(_entities, entity);
			}

			void Renderer::update()
			{
				for (Entity* e : _entities)
					submit(e);
			}


			const size_t Renderer::getSize() const
			{
				return sizeof(*this);
			}
		}
	}
}