#pragma once

#include "graphics/shaders/ShaderStage.h"

namespace fungine
{
	namespace graphics
	{
		namespace opengl
		{
			class OpenglShaderStage : public ShaderStage
			{
			public:
				OpenglShaderStage(const std::string& path, ShaderStageType type);
				~OpenglShaderStage();

				virtual std::vector<std::pair<std::string, ModifyableUniform>> findModifyableUniforms() override;

			private:
				unsigned int loadFromSource(const std::string& path, ShaderStageType type);
			};
		}
	}
}