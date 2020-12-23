
#include "Material.h"
#include "entities/Entity.h"
#include "components/rendering/Camera.h"
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
			const std::vector<graphics::Texture*>& textures,
			const std::string& name
		) :
			Component(name)
		{
			_shader = shader;
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

		Material::Material(
			graphics::ShaderProgram* shader,
			const std::vector<std::pair<std::string, graphics::Texture*>>& textures,
			const std::string& name
		) :
			Component(name)
		{
			_shader = shader;
			for (int i = 0; i < textures.size(); ++i)
			{
				if (i >= MATERIAL__MAX_TEXTURES)
				{
#ifdef DEBUG__MODE_FULL
					Debug::log(
						"Location: Material::Material(\n"
						"graphics::ShaderProgram * shader,\n"
						"const std::vector<std::pair<std::string, Texture*>>&textures,\n"
						"const std::vector<graphics::ShaderUniform<mml::Matrix4>>&uniforms\n"
						")\n"
						"Attempted to create Material with too many Textures. Current Material Texture limit is: " + std::to_string(MATERIAL__MAX_TEXTURES),
						DEBUG__ERROR_LEVEL__ERROR
					);
#endif
					break;
				}
				// Attempt to set all shader's texture uniforms
				if (_shader)
				{
					_shader->bind();
					for (int i = 0; i < textures.size(); ++i)
					{
						const std::pair<std::string, Texture*>& t = textures[i];
						_textures[i] = t.second;
						_shader->setUniform(_shader->getUniformLocation(t.first), i);
					}
					_shader->unbind();
				}
			}
		}

		Material::~Material()
		{
			Debug::notify_on_destroy(_name + "(Material)");
		}


		template void Material::addUniform<int>(graphics::ShaderUniform<int>& uniform);
		template void Material::addUniform<mml::IVector2>(graphics::ShaderUniform<mml::IVector2>& uniform);
		template void Material::addUniform<mml::IVector3>(graphics::ShaderUniform<mml::IVector3>& uniform);
		template void Material::addUniform<mml::IVector4>(graphics::ShaderUniform<mml::IVector4>& uniform);

		template void Material::addUniform<float>(graphics::ShaderUniform<float>& uniform);
		template void Material::addUniform<mml::Vector2>(graphics::ShaderUniform<mml::Vector2>& uniform);
		template void Material::addUniform<mml::Vector3>(graphics::ShaderUniform<mml::Vector3>& uniform);
		template void Material::addUniform<mml::Vector4>(graphics::ShaderUniform<mml::Vector4>& uniform);

		template void Material::addUniform<mml::Matrix4>(graphics::ShaderUniform<mml::Matrix4>& uniform);

		template<typename T>
		void Material::addUniform(graphics::ShaderUniform<T>& uniform)
		{
			uniform._location = _shader->getUniformLocation(uniform.getName());
			_shader->bind();
			_shader->setUniform(uniform.getLocation(), *uniform.getData());
			_uniformList.add(uniform);
			_shader->unbind();
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

		std::shared_ptr<Material> Material::create_material__default3D(
			graphics::ShaderProgram* shader,
			const std::vector<graphics::Texture*>& textures,
			const std::string& name
		)
		{
			return std::make_shared<Material>(shader, textures, name);
		}

		std::shared_ptr<Material> Material::create_material__default3D(
			graphics::ShaderProgram* shader,
			const std::vector<std::pair<std::string, graphics::Texture*>>& textures,
			const std::string& name
		)
		{
			return std::make_shared<Material>(shader, textures, name);
		}
	}
}