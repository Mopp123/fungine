#pragma once

#include "utils/myMathLib/MyMathLib.h"
#include "core/Common.h"
#include <string>

namespace fungine
{
	class ImageData
	{
	private:
		byte* _data = nullptr;

		int _width = 0;
		int _height = 0;
		int _channels = 0;


	public:

		ImageData(byte* pixelData, int width, int height, int channels);
		~ImageData();

		static ImageData* load_image(const std::string& path, bool flipY = false);

		void setColor(int x, int y, byte r, byte g, byte b, byte a);
		byte getColorChannel(int x, int y, unsigned int channel) const;

		inline const byte* getData() const { return _data; }
		inline const int getWidth() const { return _width; }
		inline const int getHeight() const { return _height; }
		inline const int getChannels() const { return _channels; }
	};
}