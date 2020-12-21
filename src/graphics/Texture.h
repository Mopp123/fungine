#pragma once

#include "utils/imgUtils/Image.h"

namespace fungine
{
	namespace graphics
	{
		enum class TextureFormat
		{
			RGB,
			RGBA,
			Red,
			Green,
			Blue,
			BGR
		};

		enum class TextureDataType
		{
			UnsignedByte,
			Float,
			UnsignedInteger
		};

		enum class TextureWrapping
		{
			Repeat,
			MirroredRepeat,
			ClampToBorder,
			ClampToEdge
		};

		enum class TextureFiltering
		{
			Linear,
			Nearest,
			LinearMipmap,
			NearestMipmap
		};

		class Texture
		{
		protected:
			unsigned int _id = 0;
			const ImageData* _imageData = nullptr; // *Note: Texture doesn't own its' image data!

		public:

			Texture() {}

			// internalFormat, width, height, enumFormat, enumType, pixelsPtr

			Texture(const ImageData* imgData) { _imageData = imgData; }

			virtual ~Texture() {}

			static Texture* create_texture(
				const ImageData* imgData, unsigned int width = 0, unsigned int height = 0, int samples = 1,
				TextureFormat internalFormat = TextureFormat::RGB,
				TextureFormat format = TextureFormat::RGB,
				TextureDataType texDataType = TextureDataType::UnsignedByte,
				TextureWrapping texWrapMode_s = TextureWrapping::Repeat, TextureWrapping texWrapMode_t = TextureWrapping::Repeat,
				TextureFiltering texFilteringMode_min = TextureFiltering::LinearMipmap, TextureFiltering texFilteringMode_mag = TextureFiltering::Linear
			);

			// Updates the texture to look like its' image data
			// If texture's underlying image data changes, the texture wont look any different, until this function is called!
			virtual void syncWithImgData() const = 0;

			inline const unsigned int& getID() const { return _id; }
			inline void setImageData(const ImageData* imgData) { _imageData = imgData; }
		};
	}
}