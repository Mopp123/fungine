
#include "Framebuffer.h"
#include "Graphics.h"
#include "opengl/OpenglFramebuffer.h"
#include "core/Debug.h"

namespace fungine
{
	namespace graphics
	{

		Framebuffer::Framebuffer(int width, int height, int samples) :
			_width(width), _height(height), _samples(samples)
		{}
		Framebuffer::~Framebuffer()
		{
			if (_resolvedMultisampleFramebuffer)
			{
				delete _resolvedMultisampleFramebuffer;
				_resolvedMultisampleFramebuffer = nullptr;
			}

			for (int i = 0; i < _colorAttachmentCount; i++)
			{
				if (_colorTextures[i])
					delete _colorTextures[i];
			}

			if (_depthTexture)
				delete _depthTexture;
		}

		Framebuffer* Framebuffer::create_framebuffer(int width, int height, bool isDefaultFramebuffer, int samples)
		{
			switch (Graphics::get_graphics_api())
			{
			case GraphicsAPI::OpenGL: return new opengl::OpenglFramebuffer(width, height, isDefaultFramebuffer, samples);
			default:
				Debug::log(
					"Framebuffer * Framebuffer::create_framebuffer(int width, int height)\n"
					"Invalid graphics api was in use.",
					DEBUG__ERROR_LEVEL__FATAL_ERROR
				);
				break;
			}

			return nullptr;
		}

		Texture* Framebuffer::getColorAttachment(unsigned int attachmentIndex) const
		{
#ifdef DEBUG__MODE_FULL
			if (attachmentIndex >= FRAMEBUFFER__MAX_COLOR_TEXTURES)
			{
				Debug::log(
					"Location: const Texture* Framebuffer::getColorAttachment(unsigned int attachmentIndex) const\n"
					"Requested 'attachmentIndex' was out of bounds. Current last accessable index is " + std::to_string(FRAMEBUFFER__MAX_COLOR_TEXTURES - 1),
					DEBUG__ERROR_LEVEL__ERROR
				);
				return nullptr;
			}
			else
			{
				if (_resolvedMultisampleFramebuffer)
				{
					return _resolvedMultisampleFramebuffer->getColorAttachment(attachmentIndex);
				}
				else
				{
					return _colorTextures[attachmentIndex];
				}
			}
#else
			if (_resolvedMultisampleFramebuffer)
			{
				return _resolvedMultisampleFramebuffer->getColorAttachment(attachmentIndex);
			}
			else
			{
				return _colorTextures[attachmentIndex];
			}
#endif
		}
	}
}