
#include "Component.h"
#include "entities/Entity.h"

namespace fungine
{
	namespace components
	{
		std::vector<Component*> Component::s_allComponentsPool;

		Component::Component(entities::Entity* entity)
		{
			_entity = entity;
			_id = s_allComponentsPool.size();
			add_to_dynamic_list(s_allComponentsPool, this);
		}

		Component::~Component()
		{
			s_allComponentsPool.erase(std::find(s_allComponentsPool.begin(), s_allComponentsPool.end(), this)); // Not sure if this works?
		}
	}
}