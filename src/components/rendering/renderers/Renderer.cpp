
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
			std::vector<Renderer*> Renderer::s_allRenderers;
			Framebuffer* Renderer::s_framebuffer = nullptr;

			Renderer::Renderer(bool renderShadows, const std::string& name) :
				Component(name), _renderShadows(renderShadows)
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