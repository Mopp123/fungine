
#include "Component.h"
#include "entities/Entity.h"

namespace fungine
{
	namespace components
	{
		std::vector<Component*> Component::s_allComponentsPool;
		std::vector<Component*> Component::s_allStaticComponentsPool;

		Component::Component(const std::string& name, bool isStatic) : 
			_name(name), _isStatic(isStatic)
		{
			if (!_isStatic)
			{
				_id = s_allComponentsPool.size();
				add_to_dynamic_list(s_allComponentsPool, this);
			}
			else
			{
				_id = s_allStaticComponentsPool.size();
				add_to_dynamic_list(s_allStaticComponentsPool, this);
			}
		}

		Component::~Component()
		{
			if(!_isStatic)
				s_allComponentsPool.erase(std::find(s_allComponentsPool.begin(), s_allComponentsPool.end(), this)); // Not sure if this works?
			else
				s_allStaticComponentsPool.erase(std::find(s_allStaticComponentsPool.begin(), s_allStaticComponentsPool.end(), this)); // Not sure if this works?
		}
	}
}