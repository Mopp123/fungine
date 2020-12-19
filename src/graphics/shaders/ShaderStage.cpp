
#include "ShaderStage.h"
#include "graphics/Graphics.h"
#include "graphics/opengl/shaders/OpenglShaderStage.h"
#include "core/Debug.h"

namespace fungine
{
	namespace graphics
	{
		// We allow ShaderStage creation only through the function "create_shader_stage".
		//	-> make operator new inaccessable for outsiders
		void* ShaderStage::operator new(size_t size)
		{
			return malloc(size);
		}

		ShaderStage::ShaderStage(ShaderStageType stageType) :
			_type(stageType)
		{}

		ShaderStage* ShaderStage::create_shader_stage(const std::string& path, ShaderStageType type)
		{
			switch (Graphics::get_graphics_api())
			{
			case GraphicsAPI::OpenGL: return new opengl::OpenglShaderStage(path, type);
			default:
				Debug::log(
					"Location: ShaderStage* ShaderStage::create_shader_component(const std::string& path, ShaderStageType shaderStage)\n"
					"Invalid graphics api was in use.",
					DEBUG__ERROR_LEVEL__FATAL_ERROR
				);
				break;
			}
			return nullptr;
		}
	}
}