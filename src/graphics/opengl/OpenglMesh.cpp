
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

			OpenglMesh::OpenglMesh(std::vector<VertexBuffer*> vertexBuffers, IndexBuffer* indexBuffer, DrawType drawType, const std::string& name) :
				Mesh(vertexBuffers, indexBuffer, drawType, name)
			{
				GL_FUNC(glGenVertexArrays(1, &_vaoID));

				GL_FUNC(glBindVertexArray(_vaoID));
				GL_FUNC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer->getID()));

				for (VertexBuffer* vb : _vertexBuffers)
				{
					GL_FUNC(glBindBuffer(GL_ARRAY_BUFFER, vb->getID()));

					// Offset of a single "vertex buffer element's" data in the current vertex buffer
					size_t byteOffset = 0;

					for (const VertexBufferElement& vbElement : vb->getLayout().getElements())
					{
						const unsigned int& slot = vbElement.getSlot();
						int		componentCount = 0;
						size_t	componentSize = 0;
						GLenum	componentType = 0;

						get_shader_data_type_component_count(vbElement.getShaderDataType(), componentCount);
						get_shader_data_type_size(vbElement.getShaderDataType(), componentSize);
						convert_to_GLenum__shader_data_type(vbElement.getShaderDataType(), componentType);

						GL_FUNC(glEnableVertexAttribArray(slot));
						GL_FUNC(glVertexAttribPointer(slot, componentCount, componentType, GL_FALSE, vb->getLayout().getStride(), (const void*)byteOffset));
						GL_FUNC(glDisableVertexAttribArray(vbElement.getSlot()));

						byteOffset += componentSize;
					}

				}

				GL_FUNC(glBindVertexArray(0));

			}

			OpenglMesh::~OpenglMesh()
			{
				Debug::notify_on_destroy("OpenglMesh");
			}
		}
	}
}