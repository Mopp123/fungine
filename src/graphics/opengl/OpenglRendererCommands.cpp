
#include <GLEW/glew.h>
#include "core/window/Window.h"
#include "core/Program.h"
#include "OpenglRendererCommands.h"
#include "core/Debug.h"

namespace fungine
{
	using namespace core;
	using namespace components;

	namespace graphics
	{
		namespace opengl
		{


			OpenglRendererCommands::OpenglRendererCommands()
			{}
			OpenglRendererCommands::~OpenglRendererCommands()
			{}

			void OpenglRendererCommands::init() const
			{
				GL_FUNC(glEnable(GL_DEPTH_TEST));
				if (core::Program::get_window()->getMSAASamples() > 0)
				{
					GL_FUNC(glEnable(GL_MULTISAMPLE));
				}
				clear();
				GL_FUNC(glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w));

				// Disable because text rendering..
				GL_FUNC(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
			}

			void OpenglRendererCommands::clear() const
			{
				// *TEMPORARY -> move this somewhere else in the future..
				GL_FUNC(glEnable(GL_BLEND));
				GL_FUNC(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
				GL_FUNC(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
			}

			void OpenglRendererCommands::bindShader(ShaderProgram* shader) const
			{
				GL_FUNC(glUseProgram(shader->getID()));
			}
			void OpenglRendererCommands::unbindShader() const
			{
				GL_FUNC(glUseProgram(0));
			}

			void OpenglRendererCommands::cullFace(CullFace face) const
			{
				switch (face)
				{
				case CullFace::None :
					glDisable(GL_CULL_FACE);
					break;
				case CullFace::Back:
					glEnable(GL_CULL_FACE);
					glCullFace(GL_BACK);
					break;
				case CullFace::Front:
					glEnable(GL_CULL_FACE);
					glCullFace(GL_FRONT);
					break;
				}
			}

			void OpenglRendererCommands::bindTexture(const Texture* const texture, unsigned int slot) const
			{
				GLenum glTextureSlot = GL_NONE;
				convert_to_GLenum__texture_slot(slot, glTextureSlot);
				GL_FUNC(glActiveTexture(glTextureSlot));
				GL_FUNC(glBindTexture(GL_TEXTURE_2D, texture->getID()));
			}
			void OpenglRendererCommands::unbindTexture(const Texture* const texture, unsigned int slot) const
			{
				GLenum glTextureSlot = GL_NONE;
				convert_to_GLenum__texture_slot(slot, glTextureSlot);
				GL_FUNC(glActiveTexture(glTextureSlot));
				GL_FUNC(glBindTexture(GL_TEXTURE_2D, 0));
			}

			void OpenglRendererCommands::bindMesh(const Mesh* const mesh) const
			{
				GL_FUNC(glBindVertexArray(mesh->getAPISpecific_vaoID()));

				// Enable required vertex attributes
				for (const VertexBuffer<float>* vb : mesh->getVertexBuffers())
				{
					for (const VertexBufferElement& vbElem : vb->getLayout().getElements())
						GL_FUNC(glEnableVertexAttribArray(vbElem.getSlot()));
				}
			}
			void OpenglRendererCommands::unbindMesh(const Mesh* const mesh) const
			{
				// Enable required vertex attributes
				for (const VertexBuffer<float>* vb : mesh->getVertexBuffers())
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
			void OpenglRendererCommands::drawIndices_instanced(const Mesh* const mesh, unsigned int instanceCount) const
			{
				GLenum openglDrawType = GL_NONE;
				convert_to_GLenum__draw_type(mesh->getDrawType(), openglDrawType);
				GL_FUNC(glDrawElementsInstanced(openglDrawType, mesh->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, NULL, instanceCount));
			}

			
			
			template<typename T>
			static void upload_uniform(ShaderProgram* shader, const std::vector<ShaderUniform<T>>& uniforms)
			{
				for (const ShaderUniform<T>& u : uniforms)
				{
					const int& location = u.location;
#ifdef DEBUG__MODE_FULL
					if (location <= -1)
					{
						Debug::log(
							"Location: (OpenglRendererCommands) template<typename T> static void upload_uniform(\n"
							"ShaderProgram * shader,\n"
							"const std::vector<ShaderUniform<T>>&uniforms\n"
							")\n"
							"Invalid uniform location!",
							DEBUG__ERROR_LEVEL__ERROR
						);
						continue;
					}
#endif

					shader->setUniform(location, u.data);
				}
			}
			void OpenglRendererCommands::bindMaterial(Material* const material) const
			{
				ShaderProgram* shader = material->getShader();
				bindShader(shader);

				// Bind all textures
				for (int i = 0; i < MATERIAL__MAX_TEXTURES; ++i)
				{
					const Texture* const texture = material->getTexture(i);
					if (texture)
						bindTexture(texture, i);
				}

				const UniformList& matUniformList = material->getUniformList();
				upload_uniform<int>(shader, matUniformList.getUniforms_Int());
				upload_uniform<mml::IVector2>(shader, matUniformList.getUniforms_Int2());
				upload_uniform<mml::IVector3>(shader, matUniformList.getUniforms_Int3());
				upload_uniform<mml::IVector4>(shader, matUniformList.getUniforms_Int4());

				upload_uniform<float>(shader, matUniformList.getUniforms_Float());
				upload_uniform<mml::Vector2>(shader, matUniformList.getUniforms_Float2());
				upload_uniform<mml::Vector3>(shader, matUniformList.getUniforms_Float3());
				upload_uniform<mml::Vector4>(shader, matUniformList.getUniforms_Float4());

				upload_uniform<mml::Matrix4>(shader, matUniformList.getUniforms_Matrix4());
			}
			void OpenglRendererCommands::unbindMaterial(const Material* const material) const
			{
				// unbind all used textures
				for (int i = 0; i < MATERIAL__MAX_TEXTURES; ++i)
				{
					const Texture* const texture = material->getTexture(i);
					if (texture)
						unbindTexture(texture, i);
				}
				unbindShader();
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
				GL_FUNC(glViewport(0, 0, Window::get_width(), Window::get_height()));
			}
		}
	}
}