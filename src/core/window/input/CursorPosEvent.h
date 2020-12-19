#pragma once

namespace fungine
{
	namespace core
	{
		class CursorPosEvent
		{
		public:
			virtual ~CursorPosEvent() {}
			virtual void callbackFunc(double x, double y) = 0;
		};
	}
}