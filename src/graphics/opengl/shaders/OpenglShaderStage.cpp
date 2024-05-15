
#include <GL/glew.h>
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

			static ShaderDataType to_uniform_data_type(const std::string& uniformDataType)
			{
				if (uniformDataType == "float")		return ShaderDataType::Float;
				if (uniformDataType == "vec2")		return ShaderDataType::Float2;
				if (uniformDataType == "vec3")		return ShaderDataType::Float3;
				if (uniformDataType == "vec4")		return ShaderDataType::Float4;

				if (uniformDataType == "int")		return ShaderDataType::Int;

				if (uniformDataType == "mat4")		return ShaderDataType::Matrix4;

				if (uniformDataType == "sampler2D") return ShaderDataType::Texture2D;

				return ShaderDataType::None;
			}

			std::vector<std::pair<std::string, ModifyableUniform>> OpenglShaderStage::findModifyableUniforms()
			{
				std::vector<std::pair<std::string, ModifyableUniform>> uniforms;

				std::string s = _sourceCode;

				while (!s.empty())
				{
					size_t index = 0;
					std::string line;
					for (char c : s)
					{
						index++;
						if (c == '\n' || c == '\r')
							continue;

						if (c != ';')
							line += c;
						else
							break;
					}

					s.erase(s.begin(), s.begin() + index);

					size_t uPos = line.find("uniform ");
					if (uPos != std::string::npos)
					{
						// word "uniform" should be the first thing on the line
						size_t startPos = 8;
						// find next word..
						std::string dataType;
						std::string name;

						bool lastPart = false;
						for (int i = startPos; i < line.size(); i++)
						{
							const char& c = line[i];
							if (c == ' ')
							{
								lastPart = true;
								continue;
							}

							lastPart ? name += c : dataType += c;
						}
						if (name[0] == 'm' && name[1] == '_')
						{
							ModifyableUniform mu = { name, -1, to_uniform_data_type(dataType) };
							uniforms.push_back(std::make_pair(name, mu));
						}
					}
				}
				_sourceCode.clear();
				return uniforms;
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

				_sourceCode = shaderSource;
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
