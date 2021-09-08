
#include "Texture.h"
#include "Graphics.h"
#include "core/Debug.h"
#include "graphics/opengl/OpenglTexture.h"

namespace fungine
{
	namespace graphics
	{
		Texture* Texture::create_texture(
			const ImageData* imgData, unsigned int width, unsigned int height, int samples,
			TextureFormat internalFormat,
			TextureFormat format,
			TextureDataType texDataType,
			TextureWrapping texWrapMode_s, TextureWrapping texWrapMode_t,
			TextureFiltering texFilteringMode_min, TextureFiltering texFilteringMode_mag
		)
		{
			unsigned int w = width > 0 ? width : imgData->getWidth();
			unsigned int h = height > 0 ? height : imgData->getHeight();

			switch (Graphics::get_graphics_api())
			{
			case GraphicsAPI::OpenGL: return new opengl::OpenglTexture(
				imgData,
				w, h, samples,
				internalFormat,
				format,
				texDataType,
				texWrapMode_s, texWrapMode_t,
				texFilteringMode_min, texFilteringMode_mag
			);
			default:
				Debug::log(
					"Location: Texture* Texture::create_texture(\n"
					"const ImageData * imgData,\n"
					"TextureFormat internalFormat,\n"
					"unsigned int width, unsigned int height,\n"
					"TextureFormat format,\n"
					"TextureDataType texDataType,\n"
					"TextureWrapping texWrapMode_s, TextureWrapping texWrapMode_t,\n"
					"TextureFiltering texFilteringMode_min, TextureFiltering texFilteringMode_mag\n"
					")\n"
					"Invalid graphics api was in use.",
					DEBUG__ERROR_LEVEL__FATAL_ERROR
				);
				break;
			}

			return nullptr;
		}

		Texture* Texture::create_texture(
			const ImageData* imgData,
			TextureWrapping texWrapMode_s, TextureWrapping texWrapMode_t,
			TextureFiltering texFilteringMode
		)
		{
			const unsigned int w = imgData->getWidth();
			const unsigned int h = imgData->getHeight();

			const unsigned int channels = imgData->getChannels();
			TextureFormat texFormat = TextureFormat::Red;
			switch (channels)
			{
			case 1: texFormat = TextureFormat::Red; break;
			case 3: texFormat = TextureFormat::RGB; break;
			case 4: texFormat = TextureFormat::RGBA; break;
			default:
				Debug::log(
					"Location: Texture* Texture::create_texture(\n"
					"const ImageData * imgData,\n"
					"TextureWrapping texWrapMode_s, TextureWrapping texWrapMode_t,\n"
					"TextureFiltering texFilteringMode\n"
					")\n"
					"Tried to create texture from ImageData with invalid 'color channels' count!\n"
					"InputtedImageData's color channel count was " + std::to_string(channels) + ". Currently accepted color channel counts may be 1,3 or 4!",
					DEBUG__ERROR_LEVEL__ERROR
				);
				return nullptr;
			}

			switch (Graphics::get_graphics_api())
			{
			case GraphicsAPI::OpenGL: return new opengl::OpenglTexture(
				imgData,
				w, h, 1,
				texFormat,
				texFormat,
				TextureDataType::UnsignedByte,
				texWrapMode_s, texWrapMode_t,
				texFilteringMode, texFilteringMode
			);
			default:
				Debug::log(
					"Location: Texture* Texture::create_texture(\n"
					"const ImageData * imgData,\n"
					"TextureFormat internalFormat,\n"
					"unsigned int width, unsigned int height,\n"
					"TextureFormat format,\n"
					"TextureDataType texDataType,\n"
					"TextureWrapping texWrapMode_s, TextureWrapping texWrapMode_t,\n"
					"TextureFiltering texFilteringMode_min, TextureFiltering texFilteringMode_mag\n"
					")\n"
					"Invalid graphics api was in use.",
					DEBUG__ERROR_LEVEL__FATAL_ERROR
				);
				break;
			}

			return nullptr;
		}


		TextureAtlas::TextureAtlas(
			const ImageData* imgdata, int tileCount,

			TextureWrapping texWrapMode_s, TextureWrapping texWrapMode_t,
			TextureFiltering texFilteringMode
		) : 
			_tileCount(tileCount)
		{
			_texture = Texture::create_texture(imgdata, texWrapMode_s, texWrapMode_t, texFilteringMode);
		}
		
		TextureAtlas::~TextureAtlas()
		{
			delete _texture;
		}
	}
}