
#include "GUIPanel.h"

#include "core/Debug.h"

namespace fungine
{
	namespace components
	{

		GUIPanel::GUIPanel(GUIImage* backgroundImg, GUIImage* titleBarImg) : 
			Component("GUIPanel", true), _backgroundImg(backgroundImg), _titleBarImg(titleBarImg)
		{}

		GUIPanel::~GUIPanel()
		{
			Debug::notify_on_destroy(_name);
		}

		const size_t GUIPanel::getSize() const
		{
			return sizeof(*this);
		}
	}
}