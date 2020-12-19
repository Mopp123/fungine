
#include "ShaderProgram.h"
#include "graphics/opengl/shaders/OpenglShaderProgram.h"
#include "graphics/Graphics.h"
#include "core/Debug.h"

namespace fungine
{
	namespace graphics
	{
		// We allow ShaderProgram creation only through the function "create_shader_program".
		//	-> make operator new inaccessable for outsiders
		void* ShaderProgram::operator new(size_t size)
		{
			return malloc(size);
		}

		ShaderProgram::ShaderProgram(const std::string& name) :
			_name(name)
		{}

		ShaderProgram* ShaderProgram::create_shader_program(
			const std::string& name,
			ShaderStage* vertexShader,
			ShaderStage* pixelShader,
			ShaderStage* geometryShader
		)
		{
			switch (Graphics::get_graphics_api())
			{
			case GraphicsAPI::OpenGL: return new opengl::OpenglShaderProgram(name, vertexShader, pixelShader, geometryShader);
			default:
				Debug::log(
					"ShaderProgram* ShaderProgram::create_shader_program(\n"
					"const std::string & name,\n"
					"ShaderStage * vertexShader,\n"
					"ShaderStage * pixelShader,\n"
					"ShaderStage * geometryShader\n"
					")\n"
					"Invalid graphics api was in use.",
					DEBUG__ERROR_LEVEL__ERROR
				);
				break;
			}

			return nullptr;
		}

		std::string ShaderProgram::shader_stage_to_string(ShaderStageType type)
		{
			switch (type)
			{
			case ShaderStageType::VertexShader:		return "Vertex shader";
			case ShaderStageType::PixelShader:		return "Fragment shader";
			case ShaderStageType::GeometryShader:	return "Geometry shader";
			default:								return "Invalid shader type";
			}
		}

	}
}