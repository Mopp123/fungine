
#include "Material.h"
#include "entities/Entity.h"
#include "core/Debug.h"

namespace fungine
{
	using namespace graphics;

	namespace components
	{
		// We allow Material creation only through the function "create_material".
		//	-> make operator new inaccessable for outsiders
		void* Material::operator new(size_t size)
		{
			return malloc(size);
		}

		Material::Material(
			graphics::ShaderProgram* shader,
			const std::vector<ShaderUniform<mml::Matrix4>>& uniforms,
			const std::vector<graphics::Texture*>& textures,
			entities::Entity* entity
		) :
			Component(entity)
		{
			for (int i = 0; i < textures.size(); ++i)
			{
				if (i >= MATERIAL__MAX_TEXTURES)
				{
#ifdef DEBUG__MODE_FULL
					Debug::log(
						"Location: Material::Material(\n"
						"graphics::ShaderProgram * shader,\n"
						"const std::vector<Texture*>&textures,\n"
						"const std::vector<graphics::ShaderUniform<mml::Matrix4>>&uniforms\n"
						")\n"
						"Attempted to create Material with too many Textures. Current Material Texture limit is: " + std::to_string(MATERIAL__MAX_TEXTURES),
						DEBUG__ERROR_LEVEL__ERROR
					);
#endif
					break;
				}
				_textures[i] = textures[i];
			}
		}

		Material::~Material()
		{
			Debug::notify_on_destroy("Material");
		}

		
		bool operator==(const Material& left, const Material& right)
		{
			for (int i = 0; i < MATERIAL__MAX_TEXTURES; ++i)
			{
				if (left._textures[i] != right._textures[i])
					return false;
			}
			if ((left._shader != right._shader) ||
				(left._specular_strength != right._specular_strength) ||
				(left._specular_shininess != right._specular_shininess)
			)
			{
				return false;
			}
			return true;
		}
		bool operator!=(const Material& left, const Material& right)
		{
			return !(left == right);
		}

		const size_t Material::getSize() const
		{
			return sizeof(*this);
		}

		void Material::setTexture(unsigned int textureSlot, Texture* texture)
		{
#ifdef DEBUG__MODE_FULL
			if (textureSlot >= MATERIAL__MAX_TEXTURES)
			{
				Debug::log(
					"Location: void Material::setTexture(unsigned int textureSlot, Texture * texture)\n"
					"Inputted texture slot(" + std::to_string(textureSlot) + ") was out of bounds.",
					DEBUG__ERROR_LEVEL__ERROR
				);
			}
			else
			{
				_textures[textureSlot] = texture;
			}
#else
			_textures[textureSlot] = texture;
#endif
		}

		const Texture* Material::getTexture(unsigned int textureSlot) const
		{
#ifdef DEBUG__MODE_FULL
			if (textureSlot >= MATERIAL__MAX_TEXTURES)
			{
				Debug::log(
					"Location: const Texture * Material::getTexture(unsigned int textureSlot) const\n"
					"Inputted texture slot(" + std::to_string(textureSlot) + ") was out of bounds.",
					DEBUG__ERROR_LEVEL__ERROR
				);
				return nullptr;
			}
			else
			{
				return _textures[textureSlot];
			}
#else
			return _textures[textureSlot];
#endif
		}

		std::shared_ptr<Material> Material::create_material(
			ShaderProgram* shader,
			const std::vector<ShaderUniform<mml::Matrix4>>& uniforms,
			const std::vector<Texture*>& textures
		)
		{
			return std::make_shared<Material>(shader, uniforms, textures);
		}
	}
}