#pragma once

#include "ShaderProgram.h"
#include <string>

namespace fungine
{
	namespace graphics
	{
		class ShaderStage
		{
		protected:
			unsigned int _id = 0;
			ShaderStageType _type = ShaderStageType::VertexShader;

			// We allow ShaderStage creation only through the function "create_shader_stage".
			//	-> make operator new inaccessable for outsiders
			void* operator new(size_t size);

		public:

			ShaderStage(ShaderStageType type);
			virtual ~ShaderStage() {}

			static ShaderStage* create_shader_stage(const std::string& path, ShaderStageType type);

			inline const unsigned int& getID() const { return _id; }
			inline const ShaderStageType& getStageType() const { return _type; }
		};
	}
}