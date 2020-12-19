#pragma once

#include "Texture.h"

#define FRAMEBUFFER__MAX_COLOR_TEXTURES 4

namespace fungine
{
	namespace graphics
	{
		class Framebuffer
		{
		protected:
			unsigned int _id = 0;
			unsigned int _depthRenderBufferID = 0;

			int _width = 0;
			int _height = 0;
			int _samples = 1;

			Texture* _colorTextures[FRAMEBUFFER__MAX_COLOR_TEXTURES] = { nullptr };
			unsigned int _colorAttachmentCount = 0;

			// If this framebuffer is multisampled we need another "regular" framebuffer with "regular" color attachment
			// to be able to get the actual multisampled "color texture" from this multisampled framebuffer
			Framebuffer* _resolvedMultisampleFramebuffer = nullptr;

		public:

			Framebuffer(int width, int height, int samples = 1);
			virtual ~Framebuffer();

			virtual Texture* addColorAttachment() = 0;
			virtual void createDepthAttachment() = 0;

			static Framebuffer* create_framebuffer(int width, int height, bool isDefaultFramebuffer = false, int samples = 1);

			Texture* getColorAttachment(unsigned int attachmentIndex) const;

			inline const unsigned int& getID() const { return _id; }
			inline const unsigned int& getID_resolvedMultisampleFramebuffer() const { return _resolvedMultisampleFramebuffer->getID(); }
			inline const int& getWidth() const { return _width; }
			inline const int& getHeight() const { return _height; }

			inline bool isMultisampled() const { return _samples > 1; }
		};
	}
}