#pragma once

#include "graphics/shaders/ShaderProgram.h"
#include "OpenglShaderStage.h"
#include "utils/myMathLib/MyMathLib.h"
#include <unordered_map>
#include <string>

namespace fungine
{
	namespace graphics
	{
		namespace opengl
		{
			class OpenglShaderProgram : public ShaderProgram
			{
			private:

				OpenglShaderStage* _vertexShader = nullptr;
				OpenglShaderStage* _fragmentShader = nullptr;
				OpenglShaderStage* _geometryShader = nullptr;

				// key = uniform name, value = uniform location
				std::unordered_map<std::string, int> _uniformCache;

			public:

				OpenglShaderProgram(
					const std::string& name,
					ShaderStage* vertexShader = nullptr,
					ShaderStage* pixelShader = nullptr,
					ShaderStage* geometryShader = nullptr
				);

				~OpenglShaderProgram();

				virtual int getUniformLocation(const std::string& uniformName) override;
				virtual void setUniform(int location, int val) const override;
				virtual void setUniform(int location, float val) const override;
				virtual void setUniform(int location, const mml::Vector2& vec) const override;
				virtual void setUniform(int location, const mml::Vector3& vec) const override;
				virtual void setUniform(int location, const mml::Vector4& vec) const override;
				virtual void setUniform(int location, const mml::Matrix4& m) const override;

				virtual void setUniform(const std::string& location, int val) override;
				virtual void setUniform(const std::string& location, float val) override;
				virtual void setUniform(const std::string& location, const mml::Vector2& vec) override;
				virtual void setUniform(const std::string& location, const mml::Vector3& vec) override;
				virtual void setUniform(const std::string& location, const mml::Vector4& vec) override;
				virtual void setUniform(const std::string& location, const mml::Matrix4& m) override;


				virtual void bind() override;
				virtual void unbind() override;
				virtual void cleanUp() override;
			};
		}
	}
}