
#include "Entity.h"
#include "core/Debug.h"

#include "components/common/Transform.h"
#include "components/rendering/Mesh.h"
#include "components/rendering/Material.h"
#include "components/rendering/Camera.h"
#include "components/rendering/lighting/Lights.h"
#include "components/rendering/renderers/Renderer.h"
#include "components/rendering/renderers/TerrainRenderer.h"
#include "components/rendering/renderers/NatureRenderer.h"
#include "components/guiComponents/GUIImage.h"
#include "components/guiComponents/GUIText.h"


namespace fungine
{
	using namespace components;
	using namespace rendering;

	namespace entities
	{

		Entity::Entity(bool isStatic) : 
			_isStatic(isStatic)
		{}
		Entity::~Entity()
		{
			_components.clear();
			Debug::notify_on_destroy("Entity");
		}

		void Entity::addComponent(std::shared_ptr<Component> component)
		{
			component->_entity = this;
			component->onAttackToEntity(this);
			add_to_dynamic_list(_components, component);
		}
		void Entity::removeComponent(std::shared_ptr<components::Component> component)
		{
			for (int i = 0; i < _components.size(); i++)
			{
				std::shared_ptr<Component>& c = _components[i];
				if (c == component)
				{
					_components.erase(_components.begin() + i);
					return;
				}
			}
		}

		// This is quite shit we have to do this for all possible components, if we want 
		// to have this function def in this cpp file and not in the header...
		template std::shared_ptr<Transform>			Entity::getComponent<Transform>();
		template std::shared_ptr<Mesh>				Entity::getComponent<Mesh>();
		template std::shared_ptr<Material>			Entity::getComponent<Material>();
		template std::shared_ptr<Camera>			Entity::getComponent<Camera>();
		template std::shared_ptr<DirectionalLight>	Entity::getComponent<DirectionalLight>();
		template std::shared_ptr<Renderer>			Entity::getComponent<Renderer>();
		template std::shared_ptr<BatchInstanceData>	Entity::getComponent<BatchInstanceData>();
		template std::shared_ptr<TerrainRenderer>	Entity::getComponent<TerrainRenderer>();
		template std::shared_ptr<NatureRenderer>	Entity::getComponent<NatureRenderer>();
		template std::shared_ptr<GUIImage>			Entity::getComponent<GUIImage>();
		template std::shared_ptr<GUIText>			Entity::getComponent<GUIText>();

		template std::vector<std::shared_ptr<Transform>>		Entity::getComponents<Transform>();
		template std::vector<std::shared_ptr<Mesh>>				Entity::getComponents<Mesh>();
		template std::vector<std::shared_ptr<Material>>			Entity::getComponents<Material>();
		template std::vector<std::shared_ptr<Camera>>			Entity::getComponents<Camera>();
		template std::vector<std::shared_ptr<DirectionalLight>> Entity::getComponents<DirectionalLight>();
		template std::vector<std::shared_ptr<Renderer>>			Entity::getComponents<Renderer>();
		template std::vector<std::shared_ptr<BatchInstanceData>>Entity::getComponents<BatchInstanceData>();
		template std::vector<std::shared_ptr<TerrainRenderer>>	Entity::getComponents<TerrainRenderer>();
		template std::vector<std::shared_ptr<NatureRenderer>>	Entity::getComponents<NatureRenderer>();
		template std::vector<std::shared_ptr<GUIImage>>			Entity::getComponents<GUIImage>();
		template std::vector<std::shared_ptr<GUIText>>			Entity::getComponents<GUIText>();


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