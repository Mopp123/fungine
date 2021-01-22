#pragma once

#include "graphics/Buffers.h"
#include <vector>

namespace fungine
{
	namespace graphics
	{
		namespace opengl
		{
			
			template<typename T>
			class OpenglVertexBuffer : public VertexBuffer<T>
			{
			public:
				OpenglVertexBuffer(T* data, size_t dataSize, BufferUsage usage, const VertexBufferLayout& layout);
				virtual ~OpenglVertexBuffer();

				// Updates the buffer starting from "offset" with new data
				// Mesh which owns this buffer, must be bound when calling this.
				virtual void update(int offset, size_t dataSize, const void* data) override;
			};


			class OpenglIndexBuffer : public IndexBuffer
			{
			public:
				OpenglIndexBuffer(const std::vector<unsigned int>& data);
				virtual ~OpenglIndexBuffer();
			};
		}
	}
}