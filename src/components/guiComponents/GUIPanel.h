#pragma once

#include "GUIImage.h"
#include "core/window/input/InputHandler.h"

namespace fungine
{
	namespace components
	{


		class GUIPanel : public Component
		{
		private:
			GUIImage* _backgroundImg = nullptr;
			GUIImage* _titleBarImg = nullptr; // *This is optional

		public:

			GUIPanel(GUIImage* backgroundImg, GUIImage* titleBarImg = nullptr);
			~GUIPanel();

			virtual const size_t getSize() const override;
		};
	}
}