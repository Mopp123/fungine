#pragma once

#include "graphics/Texture.h"

namespace fungine
{
	namespace graphics
	{
		namespace opengl
		{
			class OpenglTexture : public Texture
			{
			public:

				OpenglTexture(
					const ImageData* imgData,
					unsigned int width, unsigned int height, int samples,
					TextureFormat internalFormat,
					TextureFormat format,
					TextureDataType texDataType,
					TextureWrapping texWrapMode_s, TextureWrapping texWrapMode_t,
					TextureFiltering texFilteringMode_min, TextureFiltering texFilteringMode_mag
				);

				~OpenglTexture();

				// Updates the texture to look like its' image data
				// If texture's underlying image data changes, the texture wont look any different, until this function is called!
				virtual void syncWithImgData() const override;
			};
		}
	}
}