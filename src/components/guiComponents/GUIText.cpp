
#include "GUIText.h"
#include "core/resourceManaging/ResourceManager.h"

#include "core/Debug.h"

namespace fungine
{
	namespace components
	{
		
		GUIText::GUIText(std::string initStr, Font* font, float scalingFactor) :
			Component("GUIText", true), _scalingFactor(scalingFactor)
		{
			_font = font;
			setText(initStr);
		}

		GUIText::GUIText(std::string initStr, float scalingFactor) :
			Component("GUIText", true), _scalingFactor(scalingFactor)
		{
			_font = core::ResourceManager::get_default_font();
			setText(initStr);
		}
		GUIText::~GUIText()
		{
			Debug::notify_on_destroy(_name);
		}

		void GUIText::setText(const std::string& str)
		{
			_str = str;
		}

		const size_t GUIText::getSize() const
		{
			return sizeof(*this);
		}
	}
}