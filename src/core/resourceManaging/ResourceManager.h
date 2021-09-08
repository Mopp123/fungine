#pragma once

#include <unordered_map>
#include <string>
#include "graphics/Texture.h"
#include "utils/fontUtils/Font.h"

namespace fungine
{
	namespace core
	{
		class ResourceManager
		{
		private:

			// Resources which can be deleted only manually by user
			static std::unordered_map<std::string, graphics::Texture*> s_static_textures;

			static graphics::Texture* s_defaultTexture2D;
			static graphics::TextureAtlas* s_defaultTextureAtlas2D;
			static Font* s_defaultFont;
			static graphics::Texture* s_defaultTexture3D;

			static bool s_defaultResourcesCreated;

		public:

			static void create_default_resources();
			static void delete_static_resources();

			// Returns "static texture resource" with name: textureName
			static graphics::Texture* get_static_texture(const std::string& textureName);

			static graphics::Texture* get_default_texture2D();
			static graphics::TextureAtlas* get_default_textureAtlas2D();
			static Font* get_default_font();

			static graphics::Texture* get_default_texture3D();
		};
	}
}