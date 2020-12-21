#pragma once

#include "utils/myMathLib/MyMathLib.h"
#include <string>

namespace fungine
{
	namespace components
	{
		class Material;
	}

	namespace graphics
	{

		enum class UniformDataType
		{
			None,

			Float,
			Float2,
			Float3,
			Float4,

			Int,
			Int2,
			Int3,
			Int4,

			Mat4
		};

		template<typename T>
		class ShaderUniform
		{
		private:
			int _location = -1;
			std::string _name;

			T* _data;

			friend class components::Material;

		public:

			ShaderUniform(const ShaderUniform<T>& uniform) :
				_location(uniform._location), _name(uniform._name), _data(uniform._data)
			{}

			ShaderUniform(const std::string& name, T* ptrToValue) :
				_name(name), _data(ptrToValue)
			{}

			inline void setLocation(int location) { _location = location; }
			inline const int& getLocation() const { return _location; }
			inline const std::string& getName() const { return _name; }

			inline const T* const getData() const { return _data; }
		};

		struct ShaderUniformList
		{
			std::vector<ShaderUniform<int>> uniforms_int;
			std::vector<ShaderUniform<mml::IVector2>> uniforms_int2;
			std::vector<ShaderUniform<mml::IVector3>> uniforms_int3;
			std::vector<ShaderUniform<mml::IVector4>> uniforms_int4;

			std::vector<ShaderUniform<float>> uniforms_float;
			std::vector<ShaderUniform<mml::Vector2>> uniforms_float2;
			std::vector<ShaderUniform<mml::Vector3>> uniforms_float3;
			std::vector<ShaderUniform<mml::Vector4>> uniforms_float4;

			std::vector<ShaderUniform<mml::Matrix4>> uniforms_matrix4;

			void add(const ShaderUniform<int>& uniform) { uniforms_int.push_back(uniform); }
			void add(const ShaderUniform<mml::IVector2>& uniform) { uniforms_int2.push_back(uniform); }
			void add(const ShaderUniform<mml::IVector3>& uniform) { uniforms_int3.push_back(uniform); }
			void add(const ShaderUniform<mml::IVector4>& uniform) { uniforms_int4.push_back(uniform); }

			void add(const ShaderUniform<float>& uniform) {uniforms_float.push_back(uniform); }
			void add(const ShaderUniform<mml::Vector2>& uniform) { uniforms_float2.push_back(uniform); }
			void add(const ShaderUniform<mml::Vector3>& uniform) { uniforms_float3.push_back(uniform); }
			void add(const ShaderUniform<mml::Vector4>& uniform) { uniforms_float4.push_back(uniform); }
		
			void add(const ShaderUniform<mml::Matrix4>& uniform) { uniforms_matrix4.push_back(uniform); }
		};
	}
}