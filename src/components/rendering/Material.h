#pragma once

#include "components/Component.h"
#include "graphics/Texture.h"
#include "graphics/shaders/ShaderProgram.h"
#include "graphics/shaders/ShaderUniforms.h"

#include <memory>

#define MATERIAL__MAX_TEXTURES	4

namespace fungine
{
	namespace components
	{
		class Material : public Component
		{
		private:

			graphics::Texture* _textures[MATERIAL__MAX_TEXTURES] = { nullptr };
			graphics::ShaderProgram* _shader = nullptr;

			float _specular_strength =	0.0f;
			float _specular_shininess = 0.0f;

			bool _hasSpecularMap = false;
			bool _hasNormalMap = false;

			// We allow Material creation only through the function "create_material".
			//	-> make operator new inaccessable for outsiders
			void* operator new(size_t);

		public:

			Material(
				graphics::ShaderProgram* shader,
				const std::vector<graphics::ShaderUniform<mml::Matrix4>>& uniforms = {},
				const std::vector<graphics::Texture*>& textures = {},
				entities::Entity * entity = nullptr
			);
			~Material();

			void setTexture(unsigned int textureSlot, graphics::Texture* texture);
			const graphics::Texture* getTexture(unsigned int textureSlot) const;

			static std::shared_ptr<Material> create_material(
				graphics::ShaderProgram* shader,
				const std::vector<graphics::ShaderUniform<mml::Matrix4>>& uniforms = {},
				const std::vector<graphics::Texture*>& textures = {}
			);

			inline void setSpecular_strength(float val)			{ _specular_strength = val; }
			inline void setSpecular_shininess(float val)		{ _specular_shininess = val; }
			inline void setHasSpecularMap(bool arg)				{ _hasSpecularMap = arg; }
			inline void setHasNormalMap(bool arg)				{ _hasNormalMap = arg; }
			inline void setShader(graphics::ShaderProgram* shader) { _shader = shader; }
			
			inline float getSpecular_strength() const				{ return _specular_strength; }
			inline float getSpecular_shininess() const				{ return _specular_shininess; }
			inline bool hasSpecularMap() const						{ return _hasSpecularMap; }
			inline bool hasNormalMap()								{ return _hasNormalMap; }
			inline graphics::ShaderProgram* getShader()				{ return _shader; }
			
			friend bool operator==(const Material& left, const Material& right);
			friend bool operator!=(const Material& left, const Material& right);


			const size_t getSize() const override;
		};
	}
}