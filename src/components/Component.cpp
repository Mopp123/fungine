
#include "Component.h"
#include "entities/Entity.h"

namespace fungine
{
	namespace components
	{
		std::vector<Component*> Component::s_updateableComponentsPool;

		Component::Component(const std::string& name, bool isStatic, bool hasUpdateFunc) :
			_name(name), _isStatic(isStatic), _hasUpdateFunc(hasUpdateFunc)
		{
			if (_hasUpdateFunc)
			{
				add_to_dynamic_list(s_updateableComponentsPool, this);
			}
		}

		Component::~Component()
		{
			if(_hasUpdateFunc)
				s_updateableComponentsPool.erase(std::find(s_updateableComponentsPool.begin(), s_updateableComponentsPool.end(), this)); // Not sure if this works?
		}
	}
}