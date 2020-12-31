#pragma once

#include "components/Component.h"
#include "graphics/Texture.h"
#include "graphics/shaders/ShaderProgram.h"
#include "graphics/shaders/ShaderUniforms.h"
#include <utility>
#include <string>

#include <memory>

#define MATERIAL__MAX_TEXTURES	16

#define MATERIAL_DEFAULT_NAME "Material"

namespace fungine
{

	namespace components
	{
		class Material : public Component
		{
		private:



			graphics::Texture* _textures[MATERIAL__MAX_TEXTURES] = { nullptr };
			graphics::ShaderProgram* _shader = nullptr;
			// *Optinal shader to be used for shadowmap rendering.
			// If this isn't specified, uses _shader if rendered to shadowmap.
			graphics::ShaderProgram* _shadowShader = nullptr; 

			graphics::UniformList _uniformList;


			float _specular_strength =	0.0f;
			float _specular_shininess = 0.0f;

			bool _hasSpecularMap = false;
			bool _hasNormalMap = false;

			bool _twoSided = false;
			
			// We allow Material creation only through the function "create_material".
			//	-> make operator new inaccessable for outsiders
			void* operator new(size_t);

		public:

			Material(
				graphics::ShaderProgram* shader,
				const std::vector<graphics::Texture*>& textures = {},
				const std::string& name = MATERIAL_DEFAULT_NAME
			);
			Material(
				const std::vector<graphics::Texture*>& textures = {},
				const std::string& name = MATERIAL_DEFAULT_NAME
			);
			~Material();

			void setShader(graphics::ShaderProgram* shader);
			// *->TEMP
			void setShaderUniform_Int(const graphics::ShaderUniform<int>& uniform);
			void setShaderUniform_Float(const graphics::ShaderUniform<float>& uniform);
			void setShaderUniform_Float2(const graphics::ShaderUniform<mml::Vector2>& uniform);
			void setShaderUniform_Float3(const graphics::ShaderUniform<mml::Vector3>& uniform);
			void setShaderUniform_Float4(const graphics::ShaderUniform<mml::Vector4>& uniform);
			void setShaderUniform_Matrix(const graphics::ShaderUniform<mml::Matrix4>& uniform);

			graphics::ShaderUniform<int>* getShaderUniform_Int(const std::string& name);
			graphics::ShaderUniform<float>* getShaderUniform_Float(const std::string& name);
			graphics::ShaderUniform<mml::Vector2>* getShaderUniform_Float2(const std::string& name);
			graphics::ShaderUniform<mml::Vector3>* getShaderUniform_Float3(const std::string& name);
			graphics::ShaderUniform<mml::Vector4>* getShaderUniform_Float4(const std::string& name);
			graphics::ShaderUniform<mml::Matrix4>* getShaderUniform_Matrix4(const std::string& name);

			void setTexture(unsigned int textureSlot, graphics::Texture* texture);
			const graphics::Texture* getTexture(unsigned int textureSlot) const;

			static std::shared_ptr<Material> create_material__default3D(
				graphics::ShaderProgram* shader,
				const std::vector<graphics::Texture*>& textures = {},
				const std::string& name = MATERIAL_DEFAULT_NAME
			);
			static std::shared_ptr<Material> create_material__default3D(
				const std::vector<graphics::Texture*>& textures = {},
				const std::string& name = MATERIAL_DEFAULT_NAME
			);

			inline void setSpecular_strength(float val)					 { _specular_strength = val; }
			inline void setSpecular_shininess(float val)				 { _specular_shininess = val; }
			inline void setHasSpecularMap(bool arg)						 { _hasSpecularMap = arg; }
			inline void setHasNormalMap(bool arg)						 { _hasNormalMap = arg; }
			inline void setShadowShader(graphics::ShaderProgram* shader) { _shadowShader = shader; }
			inline void setTwoSided(bool arg)							 { _twoSided = arg; }

			inline float getSpecular_strength() const					{ return _specular_strength; }
			inline float getSpecular_shininess() const					{ return _specular_shininess; }
			inline bool hasSpecularMap() const							{ return _hasSpecularMap; }
			inline bool hasNormalMap()									{ return _hasNormalMap; }
			inline graphics::ShaderProgram* getShader()					{ return _shader; }
			inline graphics::ShaderProgram* getShadowShader()			{ return _shadowShader; }
			inline const graphics::UniformList& getUniformList() const  { return _uniformList; }
			inline bool isTwoSided() const								{ return _twoSided; }

			friend bool operator==(const Material& left, const Material& right);
			friend bool operator!=(const Material& left, const Material& right);


			const size_t getSize() const override;

		private:

			// Gets all modifyable uniforms from shader
			void fetchModifyableUniforms();

			// Uploads texture slots to gpu..
			void uploadTextures(const std::vector<graphics::Texture*>& textures);

			template<typename T>
			graphics::ShaderUniform<T>* findFromModifyableUniforms(
				std::vector<graphics::ShaderUniform<T>>& list,
				const std::string& uniformName
			) const;

		};
	}
}