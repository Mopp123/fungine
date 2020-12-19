
#include <glew.h>
#include "OpenglRendererCommands.h"
#include "core/Debug.h"

namespace fungine
{
	using namespace components;

	namespace graphics
	{
		namespace opengl
		{


			OpenglRendererCommands::OpenglRendererCommands()
			{}
			OpenglRendererCommands::~OpenglRendererCommands()
			{}

			void OpenglRendererCommands::clear() const
			{
				// *TEMPORARY -> move this somewhere else in the future..
				GL_FUNC(glEnable(GL_DEPTH_TEST));
				GL_FUNC(glEnable(GL_MULTISAMPLE));
				GL_FUNC(glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w));
				GL_FUNC(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
			}
			void OpenglRendererCommands::bindMesh(const Mesh* const mesh) const
			{
				GL_FUNC(glBindVertexArray(mesh->getAPISpecific_vaoID()));

				// Enable required vertex attributes
				for (const VertexBuffer* vb : mesh->getVertexBuffers())
				{
					for (const VertexBufferElement& vbElem : vb->getLayout().getElements())
						GL_FUNC(glEnableVertexAttribArray(vbElem.getSlot()));
				}
			}
			void OpenglRendererCommands::unbindMesh(const Mesh* const mesh) const
			{
				// Enable required vertex attributes
				for (const VertexBuffer* vb : mesh->getVertexBuffers())
				{
					for (const VertexBufferElement& vbElem : vb->getLayout().getElements())
					{
						GL_FUNC(glDisableVertexAttribArray(vbElem.getSlot()));
					}
				}
				GL_FUNC(glBindVertexArray(0)); // *This may actually be a little pointless..?
			}
			void OpenglRendererCommands::drawIndices(const Mesh* const mesh) const
			{
				GLenum openglDrawType = GL_NONE;
				convert_to_GLenum__draw_type(mesh->getDrawType(), openglDrawType);
				GL_FUNC(glDrawElements(openglDrawType, mesh->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, NULL));
			}
			void OpenglRendererCommands::drawIndices_instanced(const Mesh* const mesh) const
			{
				GLenum openglDrawType = GL_NONE;
				convert_to_GLenum__draw_type(mesh->getDrawType(), openglDrawType);
				GL_FUNC(glDrawElementsInstanced(openglDrawType, mesh->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, NULL, mesh->getInstanceCount()));
			}

			void OpenglRendererCommands::bindMaterial(Material* const material) const
			{
				// bind shader
				ShaderProgram* shader = material->getShader();
				shader->bind();

				// Bind all textures
				for (int i = 0; i < MATERIAL__MAX_TEXTURES; ++i)
				{
					const Texture* const texture = material->getTexture(i);
					if (texture)
					{
						GLenum glTextureSlot = GL_NONE;
						convert_to_GLenum__texture_slot(i, glTextureSlot);
						if (glTextureSlot != GL_NONE)
						{
							glActiveTexture(glTextureSlot);
							glBindTexture(GL_TEXTURE_2D, texture->getID());
						}
					}
				}
			}
			void OpenglRendererCommands::unbindMaterial(const Material* const material) const
			{
				// unbind all used textures
				for (int i = 0; i < MATERIAL__MAX_TEXTURES; ++i)
				{
					const Texture* const texture = material->getTexture(i);
					if (texture)
					{
						GLenum glTextureSlot = GL_NONE;
						convert_to_GLenum__texture_slot(i, glTextureSlot);
						if (glTextureSlot != GL_NONE)
						{
							glActiveTexture(glTextureSlot);
							glBindTexture(GL_TEXTURE_2D, 0);
						}
					}
				}

				glUseProgram(0); // "unbind shader program"
			}


			void OpenglRendererCommands::bindFramebuffer(const Framebuffer* const framebuffer) const
			{
				GL_FUNC(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->getID()));
				GL_FUNC(glViewport(0, 0, framebuffer->getWidth(), framebuffer->getHeight()));
			}
			void OpenglRendererCommands::unbindFramebuffer(const Framebuffer* const framebuffer) const
			{
				if (framebuffer)
				{
					if (framebuffer->isMultisampled())
					{
						const int& width = framebuffer->getWidth();
						const int& height = framebuffer->getHeight();

						GL_FUNC(glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer->getID()));
						GL_FUNC(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer->getID_resolvedMultisampleFramebuffer()));
						GL_FUNC(glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST)); // GL_NEAREST ? why?

					}
				}

				GL_FUNC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
			}
		}
	}
}