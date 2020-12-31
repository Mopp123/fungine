#pragma once

#include "utils/myMathLib/MyMathLib.h"
#include <string>
#include <vector>

namespace fungine
{
	namespace components
	{
		class Material;
	}

	namespace graphics
	{
		enum class ShaderDataType;
		

		template<typename T>
		struct ShaderUniform
		{
			std::string name = "None";
			int location = -1;
			ShaderDataType type = ShaderDataType::None;
			T data;

			ShaderUniform()
			{}

			ShaderUniform(const ShaderUniform<T>& uniform) :
				name(uniform.name), location(uniform.location), type(uniform.type), data(uniform.data)
			{}

			ShaderUniform(const std::string& name, int location, ShaderDataType type) :
				name(name), type(type), location(location)
			{}

			ShaderUniform(const std::string& name, ShaderDataType type, T value) :
				name(name), type(type), data(value)
			{}
		};

		class UniformList
		{
		private:

			std::vector<ShaderUniform<int>> _uniforms_Int;
			std::vector<ShaderUniform<mml::IVector2>> _uniforms_Int2;
			std::vector<ShaderUniform<mml::IVector3>> _uniforms_Int3;
			std::vector<ShaderUniform<mml::IVector4>> _uniforms_Int4;

			std::vector<ShaderUniform<float>> _uniforms_Float;
			std::vector<ShaderUniform<mml::Vector2>> _uniforms_Float2;
			std::vector<ShaderUniform<mml::Vector3>> _uniforms_Float3;
			std::vector<ShaderUniform<mml::Vector4>> _uniforms_Float4;

			std::vector<ShaderUniform<mml::Matrix4>> _uniforms_Matrix4;

			std::vector<ShaderUniform<int>> _uniforms_Texture2D;

			friend class components::Material;
			friend class ShaderProgram;

		public:

			inline const std::vector<ShaderUniform<int>>& getUniforms_Int() const { return _uniforms_Int; }
			inline const std::vector<ShaderUniform<mml::IVector2>>& getUniforms_Int2() const { return _uniforms_Int2; }
			inline const std::vector<ShaderUniform<mml::IVector3>>& getUniforms_Int3() const { return _uniforms_Int3; }
			inline const std::vector<ShaderUniform<mml::IVector4>>& getUniforms_Int4() const { return _uniforms_Int4; }

			inline const std::vector<ShaderUniform<float>>& getUniforms_Float() const { return _uniforms_Float; }
			inline const std::vector<ShaderUniform<mml::Vector2>>& getUniforms_Float2() const { return _uniforms_Float2; }
			inline const std::vector<ShaderUniform<mml::Vector3>>& getUniforms_Float3() const { return _uniforms_Float3; }
			inline const std::vector<ShaderUniform<mml::Vector4>>& getUniforms_Float4() const { return _uniforms_Float4; }

			inline const std::vector<ShaderUniform<mml::Matrix4>>& getUniforms_Matrix4() const { return _uniforms_Matrix4; }

			inline const std::vector<ShaderUniform<int>>& getUniforms_Texture2D() const { return _uniforms_Texture2D; }

			inline void clear()
			{
				_uniforms_Int.clear();
				_uniforms_Int2.clear();
				_uniforms_Int3.clear();
				_uniforms_Int4.clear();

				_uniforms_Float.clear();
				_uniforms_Float2.clear();
				_uniforms_Float3.clear();
				_uniforms_Float4.clear();

				_uniforms_Matrix4.clear();
			}
		};
	}
}