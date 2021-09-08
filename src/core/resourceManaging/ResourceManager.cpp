
#include "ResourceManager.h"
#include "core/Debug.h"

namespace fungine
{
	using namespace graphics;

	namespace core
	{

		std::unordered_map<std::string, Texture*> ResourceManager::s_static_textures;

		Texture* ResourceManager::s_defaultTexture2D = nullptr;
		TextureAtlas* ResourceManager::s_defaultTextureAtlas2D = nullptr;
		Font* ResourceManager::s_defaultFont= nullptr;
		Texture* ResourceManager::s_defaultTexture3D = nullptr;

		bool ResourceManager::s_defaultResourcesCreated = false;

		void ResourceManager::create_default_resources()
		{
			if (s_defaultResourcesCreated)
			{
				Debug::log(
					"Location: void ResourceManager::create_default_resources()\n"
					"Attempted to create default resources for the program, but the default resources were already created once!\n"
					"Default resources should be created only once in the program's lifetime!",
					DEBUG__ERROR_LEVEL__WARNING
				);
				return;
			}

			// Default font for text rendering
			s_defaultFont = new Font("res/default/fonts/TestFont.ttf", 12);

			// Create default texture image data
			byte* defaultTexture_bitmapData = new byte[3]; // 1 three channel pixel
			memset(defaultTexture_bitmapData, 255, sizeof(byte) * 3);
			ImageData defaultTexture_imgData(defaultTexture_bitmapData, 1, 1, 3);

			// Default texture and texture atlas for 2d and gui rendering
			s_defaultTexture2D = Texture::create_texture(&defaultTexture_imgData, TextureWrapping::Repeat, TextureWrapping::Repeat, TextureFiltering::Nearest);
			s_defaultTextureAtlas2D = new TextureAtlas(&defaultTexture_imgData, 1, TextureWrapping::Repeat, TextureWrapping::Repeat, TextureFiltering::Nearest);

			// Default texture for 3d rendering
			s_defaultTexture3D = Texture::create_texture(&defaultTexture_imgData);

			delete[] defaultTexture_bitmapData;

			s_defaultResourcesCreated = true;
		}

		void ResourceManager::delete_static_resources()
		{
			for (const std::pair<std::string, Texture*>& staticTextures : s_static_textures)
				delete staticTextures.second;

			Debug::log("Static resources deleted manually!");
		}

		// Returns "static texture resource" with name: textureName
		Texture* ResourceManager::get_static_texture(const std::string& textureName)
		{
			if (s_static_textures.find(textureName) != s_static_textures.end())
			{
				return s_static_textures[textureName];
			}
			else
			{
				Debug::log(
					"Location: graphics::Texture* ResourceManager::get_static_texture(const std::string& textureName)\n"
					"Failed to find static Texture Resource named: '" + textureName + "'",
					DEBUG__ERROR_LEVEL__ERROR
				);
				return nullptr;
			}
		}

		Texture* ResourceManager::get_default_texture2D()
		{
			return s_defaultTexture2D;
		}
		graphics::TextureAtlas* ResourceManager::get_default_textureAtlas2D()
		{
			return s_defaultTextureAtlas2D;
		}
		Font* ResourceManager::get_default_font()
		{
			return s_defaultFont;
		}

		Texture* ResourceManager::get_default_texture3D()
		{
			return s_defaultTexture3D;
		}
	}
}