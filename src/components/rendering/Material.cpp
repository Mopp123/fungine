
#include "Material.h"
#include "graphics/Graphics.h"
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

		static bool check_tex_slot_validity(int textureSlot)
		{
			if (textureSlot >= MATERIAL__MAX_TEXTURES)
			{
#ifdef DEBUG__MODE_FULL
				Debug::log(
					"Location: (Material) static bool check_tex_slot_validity(int textureSlot)\n"
					"Attempted to create Material with too many Textures. Current Material Texture limit is: " + std::to_string(MATERIAL__MAX_TEXTURES),
					DEBUG__ERROR_LEVEL__ERROR
				);
#endif
				return false;
			}
			return true;
		}

		Material::Material(
			graphics::ShaderProgram* shader,
			const std::vector<graphics::Texture*>& textures,
			const std::string& name
		) :
			Component(name)
		{
			for (int i = 0; i < textures.size(); ++i)
			{
				if (!check_tex_slot_validity(i))
					break;

				_textures[i] = textures[i];
			}

#ifdef DEBUG__MODE_FULL
			if (!shader)
			{
				Debug::log(
					"Location: Material::Material(\n"
					"graphics::ShaderProgram * shader,\n"
					"const std::vector<graphics::Texture*>&textures,\n"
					"const std::string & name\n"
					")\n"
					"Attempted to create material with a specific inputted shader, but the inputted shader was nullptr!",
					DEBUG__ERROR_LEVEL__ERROR
				);

			}
#endif
			_shader = shader;

			fetchModifyableUniforms();
			uploadTextures(textures);
		}

		Material::Material(
			const std::vector<graphics::Texture*>& textures,
			const std::string& name
		) :
			Component(name)
		{
			for (int i = 0; i < textures.size(); ++i)
			{
				if (!check_tex_slot_validity(i))
					break;

				_textures[i] = textures[i];
			}
		}

		Material::~Material()
		{
			Debug::notify_on_destroy(_name + "(Material)");
		}

		void Material::setShader(graphics::ShaderProgram* shader)
		{
#ifdef DEBUG__MODE_FULL
			if (!shader)
			{
				Debug::log(
					"Location: void Material::setShader(graphics::ShaderProgram* shader)\n"
					"Attempted to set Material's(" + _name + ") shader to nullptr!",
					DEBUG__ERROR_LEVEL__ERROR
				);
				return;
			}
#endif
			_uniformList.clear();
			_shader = shader;
			fetchModifyableUniforms();
			// Copy our previously assigned textures into format we can use "uploadTextures()"
			std::vector<Texture*> tempTextures;
			for (int i = 0; i < MATERIAL__MAX_TEXTURES; ++i)
			{
				if (_textures[i])
					tempTextures.push_back(_textures[i]);
			}
			uploadTextures(tempTextures);
		}

		
		template<typename T>
		ShaderUniform<T>* Material::findFromModifyableUniforms(
			std::vector<ShaderUniform<T>>& list,
			const std::string& uniformName
		) const
		{
			for (ShaderUniform<T>& u : list)
			{
				if (u.name == uniformName)
					return &u;
			}
#ifdef DEBUG__MODE_FULL
			Debug::log(
				"Location: template<typename T> ShaderUniform<T>*Material::findFromModifyableUniforms(\n"
				"			std::vector<ShaderUniform<T>>&list,\n"
				"			const std::string& uniformName\n"
				"			) const\n"
				"Material named: " + _name + " failed to find uniform named: " + uniformName + " from shader named: " + _shader->getName(),
				DEBUG__ERROR_LEVEL__ERROR
			);
#endif
			return nullptr;
		}
		

		void Material::setShaderUniform_Int(const graphics::ShaderUniform<int>& uniform)
		{
			ShaderUniform<int>* foundUniform = findFromModifyableUniforms<int>(_uniformList._uniforms_Int, uniform.name);
			if (foundUniform) foundUniform->data = uniform.data;
		}
		void Material::setShaderUniform_Float(const graphics::ShaderUniform<float>& uniform)
		{
			ShaderUniform<float>* foundUniform = findFromModifyableUniforms<float>(_uniformList._uniforms_Float, uniform.name);
			if (foundUniform) foundUniform->data = uniform.data;
		}
		void Material::setShaderUniform_Float2(const graphics::ShaderUniform<mml::Vector2>& uniform)
		{
			ShaderUniform<mml::Vector2>* foundUniform = findFromModifyableUniforms<mml::Vector2>(_uniformList._uniforms_Float2, uniform.name);
			if (foundUniform) foundUniform->data = uniform.data;
		}
		void Material::setShaderUniform_Float3(const graphics::ShaderUniform<mml::Vector3>& uniform)
		{
			ShaderUniform<mml::Vector3>* foundUniform = findFromModifyableUniforms<mml::Vector3>(_uniformList._uniforms_Float3, uniform.name);
			if (foundUniform) foundUniform->data = uniform.data;
		}
		void Material::setShaderUniform_Float4(const graphics::ShaderUniform<mml::Vector4>& uniform)
		{
			ShaderUniform<mml::Vector4>* foundUniform = findFromModifyableUniforms<mml::Vector4>(_uniformList._uniforms_Float4, uniform.name);
			if (foundUniform) foundUniform->data = uniform.data;
		}
		void Material::setShaderUniform_Matrix(const graphics::ShaderUniform<mml::Matrix4>& uniform)
		{
			ShaderUniform<mml::Matrix4>* foundUniform = findFromModifyableUniforms<mml::Matrix4>(_uniformList._uniforms_Matrix4, uniform.name);
			if (foundUniform) foundUniform->data = uniform.data;
		}

		ShaderUniform<int>* Material::getShaderUniform_Int(const std::string& name)
		{
			return findFromModifyableUniforms<int>(_uniformList._uniforms_Int, name);
		}
		
		ShaderUniform<float>* Material::getShaderUniform_Float(const std::string& name)
		{
			return findFromModifyableUniforms<float>(_uniformList._uniforms_Float, name);
		}
		ShaderUniform<mml::Vector2>* Material::getShaderUniform_Float2(const std::string& name)
		{
			return findFromModifyableUniforms<mml::Vector2>(_uniformList._uniforms_Float2, name);
		}
		ShaderUniform<mml::Vector3>* Material::getShaderUniform_Float3(const std::string& name)
		{
			return findFromModifyableUniforms<mml::Vector3>(_uniformList._uniforms_Float3, name);
		}
		ShaderUniform<mml::Vector4>* Material::getShaderUniform_Float4(const std::string& name)
		{
			return findFromModifyableUniforms<mml::Vector4>(_uniformList._uniforms_Float4, name);
		}
		ShaderUniform<mml::Matrix4>* Material::getShaderUniform_Matrix4(const std::string& name)
		{
			return findFromModifyableUniforms<mml::Matrix4>(_uniformList._uniforms_Matrix4, name);
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
			const std::vector<graphics::Texture*>& textures,
			const std::string& name
		)
		{
			return std::make_shared<Material>(textures, name);
		}

		// Gets all modifyable uniforms from shader
		void Material::fetchModifyableUniforms()
		{
			RendererCommands* rendererCommands = Graphics::get_renderer_commands();
			rendererCommands->bindShader(_shader);
			for (const std::pair<std::string, ModifyableUniform>& mu : _shader->getModifyableUniforms())
			{
				const std::string& uniformName = mu.first;
				int location = _shader->getUniformLocation(uniformName);
				switch (mu.second.dataType)
				{
				case ShaderDataType::Int: _uniformList._uniforms_Int.push_back({ uniformName, location, ShaderDataType::Int }); break;
				case ShaderDataType::Int2: _uniformList._uniforms_Int2.push_back({ uniformName, location, ShaderDataType::Int2 }); break;
				case ShaderDataType::Int3: _uniformList._uniforms_Int3.push_back({ uniformName, location, ShaderDataType::Int3 }); break;
				case ShaderDataType::Int4: _uniformList._uniforms_Int4.push_back({ uniformName, location, ShaderDataType::Int4 }); break;

				case ShaderDataType::Float: _uniformList._uniforms_Float.push_back({ uniformName, location, ShaderDataType::Float }); break;
				case ShaderDataType::Float2: _uniformList._uniforms_Float2.push_back({ uniformName, location, ShaderDataType::Float2 }); break;
				case ShaderDataType::Float3: _uniformList._uniforms_Float3.push_back({ uniformName, location, ShaderDataType::Float3 }); break;
				case ShaderDataType::Float4: _uniformList._uniforms_Float4.push_back({ uniformName, location, ShaderDataType::Float4 }); break;

				case ShaderDataType::Matrix4: _uniformList._uniforms_Matrix4.push_back({ uniformName, location, ShaderDataType::Matrix4 }); break;

				case ShaderDataType::Texture2D: _uniformList._uniforms_Texture2D.push_back({ uniformName, location, ShaderDataType::Texture2D }); break;

				default:
					break;
				}
			}
			rendererCommands->bindShader(_shader);
		}


		// Uploads texture slots to gpu..
		void Material::uploadTextures(const std::vector<Texture*>& textures)
		{
			RendererCommands* rendererCommands = Graphics::get_renderer_commands();
			rendererCommands->bindShader(_shader);
			std::vector<ShaderUniform<int>> textureUniforms = _uniformList._uniforms_Texture2D;
			
			for (int i = 0; i < textureUniforms.size(); ++i)
				_shader->setUniform(textureUniforms[i].location, i);

			rendererCommands->unbindShader();
		}
	}
}