
#include "Entity.h"
#include "core/Debug.h"

#include "components/common/Transform.h"
#include "components/rendering/Mesh.h"
#include "components/rendering/Material.h"
#include "components/rendering/Camera.h"
#include "components/rendering/lighting/Lights.h"
#include "components/rendering/renderers/Renderer.h"


namespace fungine
{
	using namespace components;
	using namespace rendering;

	namespace entities
	{

		Entity::Entity()
		{}
		Entity::~Entity()
		{
			Debug::notify_on_destroy("Entity");
		}

		void Entity::addComponent(std::shared_ptr<Component> component)
		{
			component->_entity = this;
			component->onAttackToEntity(this);
			add_to_dynamic_list(_components, component);
		}

		// This is quite shit we have to do this for all possible components, if we want 
		// to have this function def in this cpp file and not in the header...
		template std::shared_ptr<Transform>			Entity::getComponent<Transform>();
		template std::shared_ptr<Mesh>				Entity::getComponent<Mesh>();
		template std::shared_ptr<Material>			Entity::getComponent<Material>();
		template std::shared_ptr<Camera>			Entity::getComponent<Camera>();
		template std::shared_ptr<DirectionalLight>	Entity::getComponent<DirectionalLight>();
		template std::shared_ptr<Renderer>			Entity::getComponent<Renderer>();

		template std::vector<std::shared_ptr<Transform>>		Entity::getComponents<Transform>();
		template std::vector<std::shared_ptr<Mesh>>				Entity::getComponents<Mesh>();
		template std::vector<std::shared_ptr<Material>>			Entity::getComponents<Material>();
		template std::vector<std::shared_ptr<Camera>>			Entity::getComponents<Camera>();
		template std::vector<std::shared_ptr<DirectionalLight>> Entity::getComponents<DirectionalLight>();
		template std::vector<std::shared_ptr<Renderer>>			Entity::getComponents<Renderer>();


		// Returns first component of type T
		template<typename T>
		std::shared_ptr<T> Entity::getComponent()
		{
			std::shared_ptr<T> foundComponent = nullptr;
			for (std::shared_ptr<Component> c : _components)
			{
				foundComponent = std::dynamic_pointer_cast<T>(c);
				if (foundComponent)
					break;
			}
			return foundComponent;
		}

		template<typename T>
		std::vector<std::shared_ptr<T>> Entity::getComponents()
		{
			std::vector<std::shared_ptr<T>> components;
			for (const std::shared_ptr<Component> c : _components)
			{
				std::shared_ptr<T> castedComponent = std::dynamic_pointer_cast<T>(c);
				if(castedComponent)
					add_to_dynamic_list(components, castedComponent)
			}
			return components;
		}
	}
}