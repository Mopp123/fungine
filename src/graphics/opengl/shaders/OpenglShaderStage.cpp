
#include <glew.h>
#include "OpenglShaderStage.h"
#include "core/Common.h"
#include "core/Debug.h"

namespace fungine
{
	namespace graphics
	{
		namespace opengl
		{
			OpenglShaderStage::OpenglShaderStage(const std::string& path, ShaderStageType type) :
				ShaderStage(type)
			{
				_id = loadFromSource(path, _type);
			}

			OpenglShaderStage::~OpenglShaderStage()
			{
				glDeleteShader(_id);
				Debug::notify_on_destroy("Opengl" + ShaderProgram::shader_stage_to_string(_type));
			}

			unsigned int OpenglShaderStage::loadFromSource(const std::string& path, ShaderStageType type)
			{
				GLenum glShaderType = 0;
				switch (type)
				{
				case ShaderStageType::VertexShader:		glShaderType = GL_VERTEX_SHADER;	break;
				case ShaderStageType::PixelShader:		glShaderType = GL_FRAGMENT_SHADER;	break;
				case ShaderStageType::GeometryShader:	glShaderType = GL_GEOMETRY_SHADER;	break;
				default:
					Debug::log(
						"Location: unsigned int OpenglShaderStage::loadFromSource(const std::string& path, ShaderStageType type)\n"
						"Invalid shader stage type."
					);
					return GL_NONE;
				}

				FILE* file = nullptr;
				fopen_s(&file, path.c_str(), "rb");
				if (!file)
				{
					Debug::log(
						"Location: unsigned int OpenglShaderStage::loadFromSource(const std::string& path, ShaderStageType type)\n"
						"Failed to read shader stage file from: " + path
					);
					return GL_NONE;
				}


				fseek(file, 0, SEEK_END);
				int fileLength = ftell(file);
				fseek(file, 0, SEEK_SET);

				size_t bufferSize = fileLength + 1;
				char* shaderSource = new char[bufferSize];
				memset(shaderSource, 0, sizeof(char) * bufferSize);

				size_t readBytes = fread_s(shaderSource, sizeof(char) * fileLength, sizeof(char), fileLength, file);
				if (readBytes != fileLength)
				{
					Debug::log(
						"Location: unsigned int OpenglShaderStage::loadFromSource(const std::string& path, ShaderStageType type)"
						"Encountered error while reading file from: " + path + ""
						"(read byte count was different than file size)",
						DEBUG__ERROR_LEVEL__ERROR
					);
				}

				GLuint shaderID = GL_FUNC(glCreateShader(glShaderType));
				GL_FUNC(glShaderSource(shaderID, 1, &shaderSource, &fileLength));

				delete[] shaderSource;

				GL_FUNC(glCompileShader(shaderID));

				int compileResult = 0;
				int infoLogLength = 0;

				GL_FUNC(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileResult));
				GL_FUNC(glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength));

				if (compileResult != GL_TRUE)
				{
					char* shaderInfoLog = new char[infoLogLength];
					GL_FUNC(glGetShaderInfoLog(shaderID, infoLogLength, &infoLogLength, shaderInfoLog));
					Debug::log(
						"Location: unsigned int OpenglShaderStage::loadFromSource(const std::string& path, ShaderStageType type)\n"
						"Failed to compile " + ShaderProgram::shader_stage_to_string(type) + " from location: " + path + "\n"
						"Shader stage info log:",
						DEBUG__ERROR_LEVEL__ERROR
					);
					printf("\n%s\n", shaderInfoLog);
					delete[] shaderInfoLog;
					return GL_NONE;
				}

				return shaderID;
			}
		}
	}
}