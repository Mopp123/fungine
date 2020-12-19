#pragma once

namespace fungine
{
	namespace core
	{
		class MouseButtonEvent
		{
		public:
			virtual ~MouseButtonEvent() {}
			virtual void callbackFunc(int button, int action, int mods) = 0;
		};
	}
}