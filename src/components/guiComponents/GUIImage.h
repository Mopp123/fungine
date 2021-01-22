#pragma once

#include "components/Component.h"
#include "graphics/Texture.h"

namespace fungine
{
	namespace components
	{
		class GUIImage : public Component
		{
		protected:
			graphics::Texture* _texture = nullptr; // *NOTE: This component doesn't own this texture, it just has pointer to it
			mml::Vector2 _textureOffset;

		public:
			GUIImage(graphics::Texture* texture);
			virtual ~GUIImage();
			
			virtual const size_t getSize() const override;

			inline const graphics::Texture* getTexture() const { return _texture; }
			inline const mml::Vector2& getTextureOffset() const { return _textureOffset; }

		};
	}
}