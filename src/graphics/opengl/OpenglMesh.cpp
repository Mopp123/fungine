
#include <GLEW/glew.h>
#include "OpenglMesh.h"
#include "graphics/Buffers.h"
#include "OpenglBuffers.h"
#include "OpenglRendererCommands.h"
#include "core/Debug.h"

namespace fungine
{
	namespace graphics
	{
		namespace opengl
		{

			OpenglMesh::OpenglMesh(std::vector<VertexBuffer<float>*> vertexBuffers, IndexBuffer* indexBuffer, DrawType drawType, unsigned int instanceCount, const std::string& name, bool isStatic) :
				Mesh(indexBuffer, drawType, instanceCount, name, isStatic)
			{
				GL_FUNC(glGenVertexArrays(1, &_vaoID));

				GL_FUNC(glBindVertexArray(_vaoID));
				
				if (_indexBuffer != nullptr)
				{
					GL_FUNC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer->getID()));
				}

				for (VertexBuffer<float>* vb : vertexBuffers)
					addVertexBuffer(vb);

				GL_FUNC(glBindVertexArray(0));

			}

			OpenglMesh::~OpenglMesh()
			{
				GL_FUNC(glDeleteVertexArrays(1, &_vaoID));
				Debug::notify_on_destroy("OpenglMesh");
			}

			void OpenglMesh::addVertexBuffer(VertexBuffer<float>* buffer)
			{
				GL_FUNC(glBindBuffer(GL_ARRAY_BUFFER, buffer->getID()));

				// Offset of a single "vertex buffer element's" data
				size_t byteOffset = 0;

				for (const VertexBufferElement& vbElement : buffer->getLayout().getElements())
				{
					const unsigned int& slot = vbElement.getSlot();
					int		componentCount = 0;
					size_t	componentSize = 0;
					GLenum	componentType = 0;

					get_shader_data_type_component_count(vbElement.getShaderDataType(), componentCount);
					get_shader_data_type_size(vbElement.getShaderDataType(), componentSize);
					convert_to_GLenum__shader_data_type(vbElement.getShaderDataType(), componentType);

					GL_FUNC(glEnableVertexAttribArray(slot));
					GL_FUNC(glVertexAttribPointer(slot, componentCount, componentType, GL_FALSE, buffer->getLayout().getStride(), (const void*)byteOffset));
					GL_FUNC(glVertexAttribDivisor(slot, vbElement.isInstanced() ? 1 : 0));
					GL_FUNC(glDisableVertexAttribArray(vbElement.getSlot()));

					byteOffset += componentSize;
				}

				GL_FUNC(glBindBuffer(GL_ARRAY_BUFFER, 0));

				_vertexBuffers.push_back(buffer);
			}
		}
	}
}