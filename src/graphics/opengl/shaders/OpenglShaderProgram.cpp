
#include <GLEW/glew.h>
#include "OpenglShaderProgram.h"
#include "core/Debug.h"
#include "core/Common.h"

#include <stdio.h>
#include <vector>

namespace fungine
{
	namespace graphics
	{
		namespace opengl
		{
			// Attempts to cast "ShaderStage" into "OpenglShaderStage"
			// Returns pointer to the opengl shader stage, if sucessful
			static OpenglShaderStage* validate_shader_type(ShaderStage* inShader, const ShaderStageType& requiredShaderType)
			{
				if (inShader == nullptr) return nullptr;

				// Make sure this shader was marked as the type we want.
				// This is to prevent following problem:
				//	-> User accidentally marks a fragment shader as vertex shader and passes it here.
				//	This causes problem thats not so easy to detect.
				if (inShader->getStageType() != requiredShaderType)
				{
					std::string requiredShaderTypeName = ShaderProgram::shader_stage_to_string(requiredShaderType);
					std::string shaderTypeName = ShaderProgram::shader_stage_to_string(inShader->getStageType());

					Debug::log(
						"Location:	OpenglShaderProgram::OpenglShaderProgram(\n"
						"Attempted to create " + requiredShaderTypeName + " from " + shaderTypeName,
						DEBUG__ERROR_LEVEL__ERROR
					);
					return nullptr;
				}

				OpenglShaderStage* outOpenglShader = dynamic_cast<OpenglShaderStage*>(inShader);
				// Downcasting fails?
				if (inShader && !outOpenglShader)
				{
					Debug::log(
						"Location:	OpenglShaderProgram::OpenglShaderProgram(\n"
						"ShaderStage * vertexShader,\n"
						"ShaderStage * pixelShader,\n"
						"ShaderStage * geometryShader\n"
						")\n"
						"Failed to cast shader stage into graphics api specific shader stage. Make sure you created the inputted shader stage correctly.",
						DEBUG__ERROR_LEVEL__ERROR
					);
					return nullptr;
				}

				return outOpenglShader;
			}

			OpenglShaderProgram::OpenglShaderProgram(
				const std::string& name,
				ShaderStage* vertexShader,
				ShaderStage* pixelShader,
				ShaderStage* geometryShader
			) :
				ShaderProgram(name)
			{
				_id = glCreateProgram();

				_vertexShader = validate_shader_type(vertexShader, ShaderStageType::VertexShader);
				_fragmentShader = validate_shader_type(pixelShader, ShaderStageType::PixelShader);
				_geometryShader = validate_shader_type(geometryShader, ShaderStageType::GeometryShader);

				if (_vertexShader)		GL_FUNC(glAttachShader(_id, _vertexShader->getID()));
				if (_fragmentShader)	GL_FUNC(glAttachShader(_id, _fragmentShader->getID()));
				if (_geometryShader)	GL_FUNC(glAttachShader(_id, _geometryShader->getID()));

				GL_FUNC(glLinkProgram(_id));
				GL_FUNC(glValidateProgram(_id));
			}

			OpenglShaderProgram::~OpenglShaderProgram()
			{
				cleanUp();
				Debug::notify_on_destroy("OpenglShaderProgram");
			}

			int OpenglShaderProgram::getUniformLocation(const std::string& uniformName)
			{
				// If this uniform name is already in cache -> return its' location
				if (_uniformCache.find(uniformName) != _uniformCache.end())
				{
					return _uniformCache[uniformName];
				}
				// If this uniform name is not yet in cache -> find it from the shader and insert it to the cache
				else
				{
					int uniformLocation = GL_FUNC(glGetUniformLocation(_id, uniformName.c_str()));
					if (uniformLocation != -1)
					{
						_uniformCache.insert(std::make_pair(uniformName, uniformLocation));
					}
					else
					{
						Debug::log(
							"Location: unsigned int OpenglShaderProgram::getUniformLocation(const std::string& uniformName)\n"
							"Failed to find uniform: " + uniformName + " from shader program named: " + _name + "\n",
							DEBUG__ERROR_LEVEL__ERROR
						);
					}
					return uniformLocation;
				}
			}

			void OpenglShaderProgram::setUniform(int location, int val) const
			{
				GL_FUNC(glUniform1i(location, val));
			}
			void OpenglShaderProgram::setUniform(int location, const mml::IVector2& val) const
			{
				GL_FUNC(glUniform2i(location, val.x, val.y));
			}
			void OpenglShaderProgram::setUniform(int location, const mml::IVector3& val) const
			{
				GL_FUNC(glUniform3i(location, val.x, val.y, val.z));
			}
			void OpenglShaderProgram::setUniform(int location, const mml::IVector4& val) const
			{
				GL_FUNC(glUniform4i(location, val.x, val.y, val.z, val.w));
			}
			void OpenglShaderProgram::setUniform(int location, float val) const
			{
				GL_FUNC(glUniform1f(location, val));
			}
			void OpenglShaderProgram::setUniform(int location, const mml::Vector2& vec) const
			{
				GL_FUNC(glUniform2f(location, vec.x, vec.y));
			}
			void OpenglShaderProgram::setUniform(int location, const mml::Vector3& vec) const
			{
				GL_FUNC(glUniform3f(location, vec.x, vec.y, vec.z));
			}
			void OpenglShaderProgram::setUniform(int location, const mml::Vector4& vec) const
			{
				GL_FUNC(glUniform4f(location, vec.x, vec.y, vec.z, vec.w));
			}
			void OpenglShaderProgram::setUniform(int location, const mml::Matrix4& m) const
			{
				GL_FUNC(glUniformMatrix4fv(location, 1, GL_FALSE, &m[0]));
			}

			void OpenglShaderProgram::setUniform(const std::string& location, int val)
			{
				GL_FUNC(glUniform1i(getUniformLocation(location), val));
			}
			void OpenglShaderProgram::setUniform(const std::string& location, float val)
			{
				GL_FUNC(glUniform1f(getUniformLocation(location), val));
			}
			void OpenglShaderProgram::setUniform(const std::string& location, const mml::Vector2& vec)
			{
				GL_FUNC(glUniform2f(getUniformLocation(location), vec.x, vec.y));
			}
			void OpenglShaderProgram::setUniform(const std::string& location, const mml::Vector3& vec)
			{
				GL_FUNC(glUniform3f(getUniformLocation(location), vec.x, vec.y, vec.z));
			}
			void OpenglShaderProgram::setUniform(const std::string& location, const mml::Vector4& vec)
			{
				GL_FUNC(glUniform4f(getUniformLocation(location), vec.x, vec.y, vec.z, vec.w));
			}
			void OpenglShaderProgram::setUniform(const std::string& location, const mml::Matrix4& m)
			{
				GL_FUNC(glUniformMatrix4fv(getUniformLocation(location), 1, GL_FALSE, &m[0]));
			}

			void OpenglShaderProgram::bind()
			{
				glUseProgram(_id);
			}
			void OpenglShaderProgram::unbind()
			{
				glUseProgram(0);
			}
			void OpenglShaderProgram::cleanUp()
			{
				unbind();

				if (_vertexShader)		glDetachShader(_id, _vertexShader->getID());
				if (_fragmentShader)	glDetachShader(_id, _fragmentShader->getID());
				if (_geometryShader)	glDetachShader(_id, _geometryShader->getID());

				delete _vertexShader;
				delete _fragmentShader;
				delete _geometryShader;

				_vertexShader = nullptr;
				_fragmentShader = nullptr;
				_geometryShader = nullptr;

				glDeleteProgram(_id);
			}
		}
	}
}