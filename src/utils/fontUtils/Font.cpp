
#include "Font.h"
#include "core/Debug.h"
#include <cmath>
#include <vector>

namespace fungine
{
	using namespace graphics;

	Font::Font(const std::string& fontFilePath, unsigned int pixelSize, mml::Vector4 color) :
		_color(color)
	{
		createFont("QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm 1234567890+-/*=_!?^'<>:.,", fontFilePath, pixelSize);
	}
	Font::~Font()
	{
		delete _textureAtlas;
	}


	void Font::createFont(const std::string& charsToLoad, const std::string& fontFilePath, unsigned int pixelSize)
	{
		_pixelSize = pixelSize;

		FT_Library freetypeLib;
		if (FT_Init_FreeType(&freetypeLib))
		{
			Debug::log(
				"Location: void Font::createFont(const std::string& charsToLoad, const std::string& fontFilePath, unsigned int pixelSize)\n"
				"Failed to init Freetype library!",
				DEBUG__ERROR_LEVEL__ERROR
			);
			return;
		}

		// Load font file
		FT_Face fontFace;
		if (FT_New_Face(freetypeLib, fontFilePath.c_str(), 0, &fontFace))
		{
			Debug::log(
				"Location: void Font::createFont(const std::string& charsToLoad, const std::string& fontFilePath, unsigned int pixelSize)\n"
				"Failed to load font file from location: " + fontFilePath,
				DEBUG__ERROR_LEVEL__ERROR
			);
			return;
		}
		FT_Set_Pixel_Sizes(fontFace, 0, pixelSize);

		// Load data from each glyph(contained in "charsToLoad") of the font
		float charArraysqrt = std::sqrt((float)charsToLoad.size());
		int textureAtlasRowCount = (int)std::ceil(charArraysqrt);
		_textureAtlasTileWidth = 1; // width in pixels of a single tile inside the font texture atlas

		int texOffsetX = 0;
		int texOffsetY = 0;

		int maxGlyphWidth = 0;
		int maxGlyphHeight = 0;

		std::vector<std::pair<unsigned char*, FontGlyphData>> loadedGlyphs;

		for (int i = 0; i < charsToLoad.size(); i++)
		{
			char c = charsToLoad[i];

			if (FT_Load_Char(fontFace, c, FT_LOAD_RENDER))
			{
				Debug::log(
					"Location: void Font::createFont(const std::string& charsToLoad, const std::string& fontFilePath, unsigned int pixelSize)\n"
					"Failed to load character '" + std::to_string(c) + "' from a font. Font file path was " + fontFilePath,
					DEBUG__ERROR_LEVEL__ERROR
				);
				continue;
			}

			FT_GlyphSlot ftGlyph = fontFace->glyph;
			int currentGlyphWidth = ftGlyph->bitmap.width;
			int currentGlyphHeight = ftGlyph->bitmap.rows;

			FontGlyphData gd =
			{
				{ texOffsetX, texOffsetY },
				{ currentGlyphWidth, currentGlyphHeight },
				{ ftGlyph->bitmap_left, ftGlyph->bitmap_top },
				ftGlyph->advance.x
			};

			maxGlyphWidth = std::max(maxGlyphWidth, currentGlyphWidth);
			maxGlyphHeight = std::max(maxGlyphHeight, currentGlyphHeight);

			// If we have space -char -> don't store any bitmap data about it..
			if (c != ' ')
			{
				const int glyphBitmapSize = currentGlyphWidth * currentGlyphHeight;
				unsigned char* bitmap = new unsigned char[glyphBitmapSize];
				memcpy(bitmap, ftGlyph->bitmap.buffer, sizeof(unsigned char) * glyphBitmapSize);
				loadedGlyphs.push_back(std::make_pair(bitmap, gd));

				texOffsetX++;
				if (texOffsetX >= textureAtlasRowCount)
				{
					texOffsetX = 0;
					texOffsetY++;
				}
			}
			_glyphMapping.insert(std::make_pair(c, gd));
		}
		// We want each glyph in the texture atlas to have perfect square space, for simplicity's sake..
		// (This results in many unused pixels tho..)
		_textureAtlasTileWidth = std::max(maxGlyphWidth, maxGlyphHeight);

		// Combine all the loaded glyphs bitmaps into a single large texture atlas
		const unsigned int combinedGlyphBitmapWidth = textureAtlasRowCount * _textureAtlasTileWidth;
		const unsigned int combinedGlyphBitmapSize = combinedGlyphBitmapWidth * combinedGlyphBitmapWidth; // font texture atlas size in bytes

		unsigned char* combinedGlyphBitmap = new unsigned char[combinedGlyphBitmapSize];
		memset(combinedGlyphBitmap, 0, combinedGlyphBitmapSize);

		for (unsigned int apy = 0; apy < combinedGlyphBitmapWidth; ++apy)
		{
			for (unsigned int apx = 0; apx < combinedGlyphBitmapWidth; ++apx)
			{
				unsigned int glyphIndexX = std::floor(apx / _textureAtlasTileWidth);
				unsigned int glyphIndexY = std::floor(apy / _textureAtlasTileWidth);

				if (glyphIndexX + glyphIndexY * textureAtlasRowCount < (unsigned int)loadedGlyphs.size())
				{
					std::pair<unsigned char*, FontGlyphData>& g = loadedGlyphs[glyphIndexX + glyphIndexY * textureAtlasRowCount];
					if (g.first == nullptr)
						continue;

					unsigned int glyphPixelX = apx % _textureAtlasTileWidth;
					unsigned int glyphPixelY = apy % _textureAtlasTileWidth;

					if (glyphPixelX < g.second.scale.x && glyphPixelY < g.second.scale.y)
						combinedGlyphBitmap[apx + apy * combinedGlyphBitmapWidth] = g.first[glyphPixelX + glyphPixelY * g.second.scale.x];
				}
			}
		}
		// delete all heap allocated glyph bitmaps
		for (std::pair<unsigned char*, FontGlyphData>& p : loadedGlyphs)
		{
			delete p.first;
			p.first = nullptr;
		}
		ImageData fontImgData(combinedGlyphBitmap, combinedGlyphBitmapWidth, combinedGlyphBitmapWidth, 1);
		_textureAtlas = new TextureAtlas(&fontImgData, textureAtlasRowCount, TextureWrapping::ClampToBorder, TextureWrapping::ClampToBorder, TextureFiltering::Linear);

		FT_Done_Face(fontFace);
		FT_Done_FreeType(freetypeLib);
	}
}
