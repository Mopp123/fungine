#pragma once

#include "core/Common.h"
#include <vector>
#include <string>

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
			std::string _name;

			entities::Entity* _entity = nullptr; // Entity, this component is attached to

			static std::vector<Component*> s_allComponentsPool; // Every single component in this program..
			static std::vector<Component*> s_allStaticComponentsPool; // Every single static component in this program..
			
			bool _isActive = true;
			bool _isStatic = false;

			friend class entities::Entity;
			friend class core::Program;

		public:

			Component(const std::string& name, bool isStatic = false);
			virtual ~Component();

			// This happens for component when entity calls "addComponent(std::shared_ptr<Component>)"
			virtual void onAttackToEntity(entities::Entity*) {}
			virtual void update() {}

			inline void setActive(bool arg) { _isActive = arg; }
			inline void setStatic(bool arg) { _isStatic = arg; }

			inline int getID() const { return _id; }
			inline const std::string& getName() const { return _name; }
			inline entities::Entity* getEntity() { return _entity; }
			inline bool isActive() const { return _isActive; }
			inline bool isStatic() const { return _isStatic; }

			// needs to be overridden, so inheriting component type returns its' actual size in bytes
			virtual const size_t getSize() const = 0;
		};
	}
}