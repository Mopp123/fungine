
#include <GLEW/glew.h>
#include "OpenglBuffers.h"
#include "graphics/Buffers.h"
#include "core/Debug.h"
#include "OpenglRendererCommands.h"

namespace fungine
{
	namespace graphics
	{
		namespace opengl
		{
			template class OpenglVertexBuffer<float>;

			template<typename T>
			OpenglVertexBuffer<T>::OpenglVertexBuffer(T* data, size_t dataSize, BufferUsage usage, const VertexBufferLayout& layout) :
				VertexBuffer<T>(data, dataSize, usage, layout)
			{
				GLenum bufferUsage = GL_NONE;
				convert_to_GLenum__buffer_usage(usage, bufferUsage);

				GL_FUNC(glGenBuffers(1, &(this->_id)));
				GL_FUNC(glBindBuffer(GL_ARRAY_BUFFER, this->_id));

				GL_FUNC(glBufferData(GL_ARRAY_BUFFER, dataSize, data, bufferUsage));

				GL_FUNC(glBindBuffer(GL_ARRAY_BUFFER, 0));
			}

			template<typename T>
			OpenglVertexBuffer<T>::~OpenglVertexBuffer()
			{
				glDeleteBuffers(1, &(this->_id));
				Debug::notify_on_destroy("OpenglVertexBuffer");
			}

			// Updates the buffer starting from "offset" with new data
			// Mesh which owns this buffer, must be bound when calling this.
			template<typename T>
			void OpenglVertexBuffer<T>::update(int offset, size_t dataSize, const void* data)
			{
				GL_FUNC(glBindBuffer(GL_ARRAY_BUFFER, this->_id));
				GL_FUNC(glBufferSubData(GL_ARRAY_BUFFER, offset, dataSize, data));
			}

			OpenglIndexBuffer::OpenglIndexBuffer(const std::vector<unsigned int>& data) :
				IndexBuffer(data)
			{
				// * We have "GL_ARRAY_BUFFER" and NOT "GL_ELEMENT_ARRAY_BUFFER" here on purpose, since 
				// "GL_ELEMENT_ARRAY_BUFFER" isn't "valid" if theres no vao bound at the moment, but this 
				// way we can bind data to buffer thats about to be bound as "GL_ELEMENT_ARRAY_BUFFER"
				GL_FUNC(glGenBuffers(1, &_id));
				GL_FUNC(glBindBuffer(GL_ARRAY_BUFFER, _id));
				GL_FUNC(glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int) * data.size(), &data[0], GL_STATIC_DRAW));
				GL_FUNC(glBindBuffer(GL_ARRAY_BUFFER, 0));
			}

			OpenglIndexBuffer::~OpenglIndexBuffer()
			{
				glDeleteBuffers(1, &_id);
				Debug::notify_on_destroy("OpenglIndexBuffer");
			}
		}
	}
}