#pragma once

#include "graphics/Buffers.h"
#include <vector>

namespace fungine
{
	namespace graphics
	{
		namespace opengl
		{
			class OpenglVertexBuffer : public VertexBuffer
			{
			public:
				OpenglVertexBuffer(const std::vector<float>& data, BufferUsage usage, const VertexBufferLayout& layout);
				~OpenglVertexBuffer();

				// Updates the buffer starting from "offset" with new data
				virtual void update(unsigned int offset, const std::vector<float>& data) override;
			};

			class OpenglIndexBuffer : public IndexBuffer
			{
			public:
				OpenglIndexBuffer(const std::vector<unsigned int>& data);
				~OpenglIndexBuffer();
			};
		}
	}
}