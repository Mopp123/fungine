
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


			OpenglVertexBuffer::OpenglVertexBuffer(const std::vector<float>& data, BufferUsage usage, const VertexBufferLayout& layout) :
				VertexBuffer(data, usage, layout)
			{
				GLenum bufferUsage = GL_NONE;
				convert_to_GLenum__buffer_usage(usage, bufferUsage);

				GL_FUNC(glGenBuffers(1, &_id));
				GL_FUNC(glBindBuffer(GL_ARRAY_BUFFER, _id));

				GL_FUNC(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), &data[0], GL_STATIC_DRAW));

				GL_FUNC(glBindBuffer(GL_ARRAY_BUFFER, 0));
			}

			OpenglVertexBuffer::~OpenglVertexBuffer()
			{
				glDeleteBuffers(1, &_id);
				Debug::notify_on_destroy("OpenglVertexBuffer");
			}

			// Updates the buffer starting from "offset" with new data
			void OpenglVertexBuffer::update(unsigned int offset, const std::vector<float>& data)
			{
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