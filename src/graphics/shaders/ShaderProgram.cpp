
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

		/* ! ! ! THIS WAS COMPLETELY BROKEN PIECE OF SHIT SYSTEM ! ! !
		  ------------------------------------------------------------
		template void ShaderProgram::addUniform<int>(const ShaderUniform<int>& uniform);
		template void ShaderProgram::addUniform<mml::IVector2>(const ShaderUniform<mml::IVector2>& uniform);
		template void ShaderProgram::addUniform<mml::IVector3>(const ShaderUniform<mml::IVector3>& uniform);
		template void ShaderProgram::addUniform<mml::IVector4>(const ShaderUniform<mml::IVector4>& uniform);

		template void ShaderProgram::addUniform<float>(const ShaderUniform<float>& uniform);
		template void ShaderProgram::addUniform<mml::Vector2>(const ShaderUniform<mml::Vector2>& uniform);
		template void ShaderProgram::addUniform<mml::Vector3>(const ShaderUniform<mml::Vector3>& uniform);
		template void ShaderProgram::addUniform<mml::Vector4>(const ShaderUniform<mml::Vector4>& uniform);

		template void ShaderProgram::addUniform<mml::Matrix4>(const ShaderUniform<mml::Matrix4>& uniform);

		template<typename T>
		void ShaderProgram::addUniform(const ShaderUniform<T>& uniform)
		{
			ShaderUniform<T> u = uniform;
			u._location = getUniformLocation(u.getName());
			RendererCommands* rendererCommands = Graphics::get_renderer_commands();
			rendererCommands->bindShader(this);
			setUniform(u.getLocation(), *u.getData());
			rendererCommands->unbindShader();
			_uniformList.add(u);
		}*/
	}
}