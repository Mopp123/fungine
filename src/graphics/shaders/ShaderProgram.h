#pragma once

#include "utils/myMathLib/MyMathLib.h"
// wtf?
//#include "ShaderUniforms.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace fungine
{
	namespace graphics
	{
		class ShaderStage; // needs forward declaring.. quite disgusting..

		enum class ShaderDataType
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
			Matrix4,
			Texture2D
		};

		enum class ShaderStageType
		{
			VertexShader,
			PixelShader,
			GeometryShader
		};

		struct ModifyableUniform
		{
			std::string name;
			int location;
			ShaderDataType dataType;
		};

		class ShaderProgram
		{
		protected:
			unsigned int _id = 0;
			std::string _name;

			// Uniforms a material using this shader may modify
			std::vector<std::pair<std::string, ModifyableUniform>> _modifyableUniforms;

			// We allow ShaderProgram creation only through the function "create_shader_program".
			//	-> make operator new inaccessable for outsiders
			void* operator new(size_t size);

		public:

			ShaderProgram(const std::string& name);
			virtual ~ShaderProgram() {}

			static ShaderProgram* create_shader_program(
				const std::string& name,
				ShaderStage* vertexShader = nullptr,
				ShaderStage* pixelShader = nullptr,
				ShaderStage* geometryShader = nullptr
			);

			virtual int getUniformLocation(const std::string& uniformName) = 0;
			virtual bool hasUniformLocation(const std::string& name) const = 0;

			virtual void setUniform(int location, const int& val) const = 0;
			virtual void setUniform(int location, const mml::IVector2&) const = 0;
			virtual void setUniform(int location, const mml::IVector3&) const = 0;
			virtual void setUniform(int location, const mml::IVector4&) const = 0;

			virtual void setUniform(int location, const float& val) const = 0;
			virtual void setUniform(int location, const mml::Vector2& vec) const = 0;
			virtual void setUniform(int location, const mml::Vector3& vec) const = 0;
			virtual void setUniform(int location, const mml::Vector4& vec) const = 0;

			virtual void setUniform(int location, const mml::Matrix4& m) const = 0;


			virtual void setUniform(const std::string& location, const int& val) = 0;
			virtual void setUniform(const std::string& location, const mml::IVector2& val) = 0;
			virtual void setUniform(const std::string& location, const mml::IVector3& val) = 0;
			virtual void setUniform(const std::string& location, const mml::IVector4& val) = 0;

			virtual void setUniform(const std::string& location, const float& val) = 0;
			virtual void setUniform(const std::string& location, const mml::Vector2& vec) = 0;
			virtual void setUniform(const std::string& location, const mml::Vector3& vec) = 0;
			virtual void setUniform(const std::string& location, const mml::Vector4& vec) = 0;

			virtual void setUniform(const std::string& location, const mml::Matrix4& m) = 0;


			static std::string shader_stage_to_string(ShaderStageType type);
			virtual void cleanUp() = 0;

			inline const unsigned int getID() const { return _id; }
			inline const std::string& getName() const { return _name; }

			inline std::vector<std::pair<std::string, ModifyableUniform>>& getModifyableUniforms() { return _modifyableUniforms; }
		};
	}
}
