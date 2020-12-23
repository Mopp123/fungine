
#include "Renderer.h"
#include "core/window/Window.h"
#include "utils/Time.h"
#include "graphics/RendererCommands.h"
#include "components/rendering/Camera.h"
#include "components/rendering/lighting/Lights.h"
#include "core/Debug.h"

namespace fungine
{
	using namespace entities;
	using namespace graphics;

	namespace components
	{
		namespace rendering
		{

			void RenderQue::add(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
			{
				// If you can find already existing batch for this mesh/material combo -> add it to that batch
				if (_batches.find(material) != _batches.end())
				{
					std::vector<std::shared_ptr<Mesh>>& batch = _batches[material];
				#ifdef DEBUG__MODE_FULL
					if (batch.empty())
					{
						Debug::log(
							"Location: void RenderQue::add(const Mesh* mesh, const Material* material)\n"
							"Triend to add mesh to an already existing batch, but the batch was empty!\n"
							"If there is a batch there should always be at least 1 mesh in its' render que.",
							DEBUG__ERROR_LEVEL__ERROR
						);
						return;
					}
				#endif
					add_to_dynamic_list(batch, mesh);
				}
				else // If we cannot find already existing batch for
				{
					std::vector<std::shared_ptr<Mesh>> newBatch;
					add_to_dynamic_list(newBatch, mesh);
					_batches.insert(std::make_pair(material, newBatch));
				}
			}

			void RenderQue::clear()
			{
				_batches.clear();
			}


			std::vector<Renderer*> Renderer::s_allRenderers;
			Framebuffer* Renderer::s_framebuffer = nullptr;

			Renderer::Renderer(const std::string& name) :
				Component(name)
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