#pragma once

#include "core/Common.h"
#include <vector>

namespace fungine
{
	namespace core
	{
		class Program;
	}
	namespace entities
	{
		class Entity;
	}
	
	namespace components
	{
		class Component
		{
		protected:
			int _id = COMPONENT__INVALID_ID;
			entities::Entity* _entity = nullptr; // Entity, this component is attached to

			static std::vector<Component*> s_allComponentsPool; // Every single component in this program..

			bool _isActive = true;
			
			friend class entities::Entity;
			friend class core::Program;

		public:

			Component(entities::Entity* entity);
			virtual ~Component();

			// This happens for component when entity calls "addComponent(std::shared_ptr<Component>)"
			virtual void onAttackToEntity(entities::Entity*) {}
			virtual void update() {}

			inline void setActive(bool arg) { _isActive = arg; }

			inline int getID() const { return _id; }
			inline entities::Entity* getEntity() { return _entity; }
			inline bool isActive() const { return _isActive; }
		
			// needs to be overridden, so inheriting component type returns its' actual size in bytes
			virtual const size_t getSize() const = 0;
		};
	}
}