#pragma once

#include "graphics/Framebuffer.h"

namespace fungine
{
	namespace graphics
	{
		namespace opengl
		{
			class OpenglFramebuffer : public Framebuffer
			{
			public:
				OpenglFramebuffer(int width, int height, bool isDefaultFramebuffer, int samples);
				~OpenglFramebuffer();

				virtual Texture* addColorAttachment() override;
				virtual Texture* createDepthAttachment() override;
			};
		}
	}
}