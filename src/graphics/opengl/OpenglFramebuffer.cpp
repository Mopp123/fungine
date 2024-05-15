
#include <GL/glew.h>
#include "OpenglFramebuffer.h"
#include "OpenglRendererCommands.h"
#include "core/Debug.h"

namespace fungine
{
	namespace graphics
	{
		namespace opengl
		{

			OpenglFramebuffer::OpenglFramebuffer(int width, int height, bool isDefaultFramebuffer, int samples) :
				Framebuffer(width, height, samples)
			{
				if (isDefaultFramebuffer)
					return;

				GL_FUNC(glGenFramebuffers(1, &_id));
				GL_FUNC(glBindFramebuffer(GL_FRAMEBUFFER, _id));
				// Create depth render buffer for this framebuffer
				// *We dont necessary always require the depth buffer here..
				GL_FUNC(glGenRenderbuffers(1, &_depthRenderBufferID));
				GL_FUNC(glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderBufferID));

				if (_samples > 1)
				{
					GL_FUNC(glRenderbufferStorageMultisample(GL_RENDERBUFFER, _samples, GL_DEPTH24_STENCIL8, _width, _height));
				}
				else
				{
					GL_FUNC(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height));
				}
				GL_FUNC(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRenderBufferID));
				GL_FUNC(glBindRenderbuffer(GL_RENDERBUFFER, 0));
				GL_FUNC(glBindFramebuffer(GL_FRAMEBUFFER, 0));

				if (_samples > 1)
				{
					// We also need another "regular" framebuffer so we can get multisampled color texture to it from this framebuffer...
					_resolvedMultisampleFramebuffer = Framebuffer::create_framebuffer(_width, _height);
					_resolvedMultisampleFramebuffer->addColorAttachment();
				}
			}

			OpenglFramebuffer::~OpenglFramebuffer()
			{
				GL_FUNC(glBindFramebuffer(GL_FRAMEBUFFER, 0));

				if (_id != GL_NONE)
					GL_FUNC(glDeleteFramebuffers(1, &_id));

				Debug::notify_on_destroy("OpenglFramebuffer");
			}

			Texture* OpenglFramebuffer::addColorAttachment()
			{
				Texture* colorTexture = Texture::create_texture(
					0, _width, _height, _samples,
					TextureFormat::RGB, TextureFormat::RGB,
					TextureDataType::UnsignedByte,
					TextureWrapping::Repeat, TextureWrapping::Repeat,
					TextureFiltering::Linear, TextureFiltering::Linear
				);

				unsigned int attachmentIndex = _colorAttachmentCount;
				GLenum glTextureTarget = _samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
				GLenum glAttachment = GL_NONE;
				convert_to_GLenum__framebuffer_color_attachment(attachmentIndex, glAttachment);

				GL_FUNC(glBindFramebuffer(GL_FRAMEBUFFER, _id));
				GL_FUNC(glBindTexture(glTextureTarget, colorTexture->getID())); // *Not sure is this required here

				GL_FUNC(glFramebufferTexture2D(GL_FRAMEBUFFER, glAttachment, glTextureTarget, colorTexture->getID(), 0));

				if (_colorTextures[attachmentIndex])
				{
					GL_FUNC(glBindTexture(glTextureTarget, 0)); // *Not sure is this required here
					GL_FUNC(glBindRenderbuffer(GL_RENDERBUFFER, 0)); // *Not sure is this required here
					GL_FUNC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
					Debug::log(
						"Location: void OpenglFramebuffer::createColorAttachment()\n"
						"Tried to create framebuffer color attachment to an already occupied slot. Slot was: " + std::to_string(attachmentIndex),
						DEBUG__ERROR_LEVEL__ERROR
					);
					delete colorTexture;
					return nullptr;
				}
				_colorTextures[attachmentIndex] = colorTexture;
				_colorAttachmentCount++;

				// finalize..
				GLenum drawBuffers[FRAMEBUFFER__MAX_COLOR_TEXTURES] =
				{
					GL_COLOR_ATTACHMENT0,
					GL_COLOR_ATTACHMENT1,
					GL_COLOR_ATTACHMENT2,
					GL_COLOR_ATTACHMENT3
				};

				GL_FUNC(glDrawBuffers(_colorAttachmentCount, drawBuffers));

				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				{
					Debug::log(
						"Location: void OpenglFramebuffer::createColorAttachment()\n"
						"Error occured while creating framebuffer color attachment.",
						DEBUG__ERROR_LEVEL__ERROR
					);
				}

				GL_FUNC(glBindTexture(glTextureTarget, 0)); // *Not sure is this required here
				GL_FUNC(glBindFramebuffer(GL_FRAMEBUFFER, 0));

				return colorTexture;
			}

			Texture* OpenglFramebuffer::createDepthAttachment()
			{
				Texture* depthTexture = Texture::create_texture(
					0, _width, _height, 1,
					TextureFormat::Depth, TextureFormat::Depth,
					TextureDataType::Float,
					TextureWrapping::ClampToBorder, TextureWrapping::ClampToBorder,
					TextureFiltering::Nearest, TextureFiltering::Nearest
				);
				// *->TEMP : quicky hack borders of the texture to white..
				float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
				GL_FUNC(glBindTexture(GL_TEXTURE_2D, depthTexture->getID()));
				GL_FUNC(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));

				GLenum glTextureTarget = GL_TEXTURE_2D;
				GLenum glAttachment = GL_DEPTH_ATTACHMENT;

				GL_FUNC(glBindFramebuffer(GL_FRAMEBUFFER, _id));
				GL_FUNC(glBindTexture(glTextureTarget, depthTexture->getID())); // *Not sure is this required here

				GL_FUNC(glFramebufferTexture2D(GL_FRAMEBUFFER, glAttachment, glTextureTarget, depthTexture->getID(), 0));

				// finalize..
				// *This is a problem, if we wanted to have color texture attachments AND depth texture attachments
				// on the same framebuffer..
				GL_FUNC(glDrawBuffer(GL_NONE));
				GL_FUNC(glReadBuffer(GL_NONE));

				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				{
					Debug::log(
						"Location: void OpenglFramebuffer::createDepthAttachment()\n"
						"Error occured while creating framebuffer color attachment.",
						DEBUG__ERROR_LEVEL__ERROR
					);
				}

				GL_FUNC(glBindTexture(glTextureTarget, 0)); // *Not sure is this required here
				GL_FUNC(glBindFramebuffer(GL_FRAMEBUFFER, 0));

				_depthTexture = depthTexture;

				return depthTexture;
			}

		}
	}
}
