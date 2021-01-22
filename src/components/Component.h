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
			std::string _name;

			entities::Entity* _entity = nullptr; // Entity, this component is attached to

			static std::vector<Component*> s_updateableComponentsPool; // Every single updateable component in this program..
			
			bool _isActive = true;
			bool _isStatic = false;
			bool _hasUpdateFunc = false;

			friend class entities::Entity;
			friend class core::Program;

		public:

			Component(const std::string& name, bool isStatic = false, bool hasUpdateFunc = false);
			virtual ~Component();

			// This happens for component when entity calls "addComponent(std::shared_ptr<Component>)"
			virtual void onAttackToEntity(entities::Entity*) {}
			virtual void update() {}

			inline void setActive(bool arg) { _isActive = arg; }
			inline void setStatic(bool arg) { _isStatic = arg; }

			inline const std::string& getName() const { return _name; }
			inline entities::Entity* getEntity() { return _entity; }
			inline bool isActive() const { return _isActive; }
			inline bool isStatic() const { return _isStatic; }
			inline bool isUpdateable() const { return _hasUpdateFunc; }

			// needs to be overridden, so inheriting component type returns its' actual size in bytes
			virtual const size_t getSize() const = 0;
		};
	}
}