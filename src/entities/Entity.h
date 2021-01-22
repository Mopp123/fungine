#pragma once

#include "components/Component.h"
#include <vector>
#include <memory>

namespace fungine
{
	namespace entities
	{
		class Entity
		{
		private:
			// *Note: Reason why these are shared ptrs is that entities can share same component, 
			// but the component still gets destroyed properly and at the right time!
			std::vector<std::shared_ptr<components::Component>> _components;
			bool _isStatic = false;

		public:

			Entity(bool isStatic = false);
			virtual ~Entity();

			// *Note: Pass by copying to increase component ptr's ref count ON PURPOSE(because entities can share components)
			void addComponent(std::shared_ptr<components::Component> component);

			// Returns first component of type T
			template<typename T>
			std::shared_ptr<T> getComponent(); // *Not sure should we increase ref count by this?

			template<typename T>
			std::vector<std::shared_ptr<T>> getComponents();

			inline bool isStatic() const { return _isStatic; }
		};
	}
}