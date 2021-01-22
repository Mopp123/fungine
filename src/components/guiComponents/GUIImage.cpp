
#include "GUIImage.h"

namespace fungine
{
	namespace components
	{

		GUIImage::GUIImage(graphics::Texture* texture) :
			Component("GUIImage", true), _texture(texture)
		{}

		GUIImage::~GUIImage()
		{}

		const size_t GUIImage::getSize() const
		{
			return sizeof(*this);
		}
	}
}