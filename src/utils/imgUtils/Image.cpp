
#include "Image.h"
#include "core/Debug.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
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


	void ImageData::setColor(int x, int y, byte r, byte g, byte b, byte a)
	{
		if (x < 0 || x >= _width || y < 0 || y >= _height)
		{
#ifdef DEBUG__MODE_FULL
			Debug::log(
				"Location: void ImageData::setColor(int x, int y, byte r, byte g, byte b, byte a)\n"
				"Invalid pixel coordinates: " + std::to_string(x) + ", " + std::to_string(y) + "\n"
				"Image's dimensions were: " + std::to_string(_width) + "x" + std::to_string(_height),
				DEBUG__ERROR_LEVEL__ERROR
			);
#endif
			return;
		}

		unsigned int pixelIndex_r = (x + y * _width) * _channels;
		unsigned int pixelIndex_g = (x + y * _width) * _channels + 1;
		unsigned int pixelIndex_b = (x + y * _width) * _channels + 2;
		unsigned int pixelIndex_a = (x + y * _width) * _channels + 3;
		_data[pixelIndex_r] = r;
		if (_channels <= 1) return;
		_data[pixelIndex_g] = g;
		if (_channels <= 2) return;
		_data[pixelIndex_b] = b;
		if (_channels <= 3) return;
		_data[pixelIndex_a] = a;
	}
	byte ImageData::getColorChannel(int x, int y, unsigned int channel) const
	{
		if (channel >= _channels)
		{
#ifdef DEBUG__MODE_FULL
			Debug::log(
				"Location: byte ImageData::getColorChannel(int x, int y, unsigned int channel) const\n"
				"Attempted to get image color value at invalid channel!\n"
				"Attempted to access channel: " + std::to_string(channel) + "\n"
				"Image had: " + std::to_string(_channels) + " channels.",
				DEBUG__ERROR_LEVEL__ERROR
			);
#endif
			return 0;
		}

		size_t imgSize = _width * _height * _channels;
		unsigned int pixelIndex = (x + y * _width) * _channels + channel;
		if (pixelIndex >= imgSize)
		{
#ifdef DEBUG__MODE_FULL
			Debug::log(
				"Location: byte ImageData::getColorChannel(int x, int y, unsigned int channel) const\n"
				"Invalid pixel coordinates: " + std::to_string(x) + ", " + std::to_string(y) + "\n"
				"Image's dimensions were: " + std::to_string(_width) + "x" + std::to_string(_height),
				DEBUG__ERROR_LEVEL__ERROR
			);
#endif
			return 0;
		}

		return _data[pixelIndex];
	}
}
