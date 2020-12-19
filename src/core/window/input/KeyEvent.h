#pragma once

namespace fungine
{
	namespace core
	{
		class KeyEvent
		{
		public:
			virtual ~KeyEvent() {}
			virtual void callbackFunc(int key, int scancode, int action, int mods) = 0;
		};
	}
}