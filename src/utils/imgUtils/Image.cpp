
#include "Image.h"
#include "core/Debug.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <string>

namespace fungine
{
	ImageData::ImageData(byte* pixelData, int width, int height, int channels) :
		_width(width), _height(height), _channels(channels)
	{
		size_t size = _width * _height * _channels;
		_data = new byte[size];
		memcpy(_data, pixelData, size);
	}

	ImageData::~ImageData()
	{
		delete[] _data;
		Debug::notify_on_destroy("ImageData");
	}


	ImageData* ImageData::load_image(const std::string& path, bool flipY)
	{
		int width = 0;
		int height = 0;
		int channels = 0;

		stbi_set_flip_vertically_on_load(flipY);

		byte* stbImageData = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (!stbImageData)
		{
			Debug::log(
				"Location: ImageData* ImageData::load_image(const std::string& path, bool flipY)\n"
				"Failed to load image from location : " + path,
				DEBUG__ERROR_LEVEL__ERROR
			);
			stbi_image_free(stbImageData);
			return nullptr;
		}

		ImageData* imgData = new ImageData(stbImageData, width, height, channels);
		stbi_image_free(stbImageData);
		return imgData;
	}
}