#pragma once

#include "graphics/Texture.h"
#include <unordered_map>

#include <ft2build.h>
#include FT_FREETYPE_H


/*
	Possible problems:
		* When creating new Font, we init freetype and also "clear"/"deallocate"(FT_Done_FreeType) freetype. This may be problem, 
		when creating and deleting multiple fonts? (Not tested!)
*/

namespace fungine
{
	struct FontGlyphData
	{
		mml::IVector2 textureOffset;// Offset in font texture atlas, where we get this glyph's visual
		mml::IVector2 scale;		// Scale of this glyph
		mml::IVector2 bearing;		// Offset from baseline to left/top of glyph
		unsigned int advance;		// Offset to advance to next glyph
	};

	class Font
	{
	private:

		graphics::TextureAtlas* _textureAtlas = nullptr;
		unsigned int _pixelSize = 1;
		int _textureAtlasTileWidth = 1; // Width in pixels of a single tile inside the font's texture atlas.

		std::unordered_map<char, FontGlyphData> _glyphMapping;

		mml::Vector4 _color;

	public:
		Font(const std::string& fontFilePath, unsigned int pixelSize, mml::Vector4 color = mml::Vector4(1,1,1,1));
		~Font();

		inline graphics::TextureAtlas* getTextureAtlas() const { return _textureAtlas; }
		inline const unsigned int getPixelSize() const { return _pixelSize; }
		inline std::unordered_map<char, FontGlyphData>& getGlyphMapping() { return _glyphMapping; }
		inline int getTilePixelWidth() const { return _textureAtlasTileWidth; }

		inline const mml::Vector4& getColor() const { return _color; }

	private:
		void createFont(const std::string& charsToLoad, const std::string& fontFilePath, unsigned int pixelSize);
	};
}