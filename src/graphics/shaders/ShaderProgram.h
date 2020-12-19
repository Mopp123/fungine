#pragma once

#include "utils/myMathLib/MyMathLib.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace fungine
{
	namespace graphics
	{
		class ShaderStage; // needs forward declaring.. quite disgusting..

		enum class ShaderStageType
		{
			VertexShader,
			PixelShader,
			GeometryShader
		};

		class ShaderProgram
		{
		protected:
			unsigned int _id = 0;
			std::string _name;

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
			virtual void setUniform(int location, int val) const = 0;
			virtual void setUniform(int location, float val) const = 0;
			virtual void setUniform(int location, const mml::Vector2& vec) const = 0;
			virtual void setUniform(int location, const mml::Vector3& vec) const = 0;
			virtual void setUniform(int location, const mml::Vector4& vec) const = 0;
			virtual void setUniform(int location, const mml::Matrix4& m) const = 0;

			virtual void setUniform(const std::string& location, int val) = 0;
			virtual void setUniform(const std::string& location, float val) = 0;
			virtual void setUniform(const std::string& location, const mml::Vector2& vec) = 0;
			virtual void setUniform(const std::string& location, const mml::Vector3& vec) = 0;
			virtual void setUniform(const std::string& location, const mml::Vector4& vec) = 0;
			virtual void setUniform(const std::string& location, const mml::Matrix4& m) = 0;


			static std::string shader_stage_to_string(ShaderStageType type);

			virtual void bind() = 0;
			virtual void unbind() = 0;
			virtual void cleanUp() = 0;


			inline const std::string& getName() const { return _name; }
		};
	}
}