#pragma once

#include "utils/myMathLib/MyMathLib.h"
#include "graphics/Framebuffer.h"

namespace fungine
{
	namespace components
	{
		class ShadowCaster
		{
		protected:

			mml::Matrix4 _projectionMatrix;
			mml::Matrix4 _viewMatrix;

			graphics::Framebuffer* _framebuffer = nullptr;
			graphics::Texture* _shadowmap;

		public:

			ShadowCaster() {}
			virtual ~ShadowCaster() {}
			virtual void init(unsigned int shadowmapWidth, unsigned int shadowmapHeight) = 0;

			virtual void update() = 0;

			inline const mml::Matrix4& getProjectionMatrix() const { return _projectionMatrix; }
			inline const mml::Matrix4& getViewMatrix() const { return _viewMatrix; }

			inline graphics::Framebuffer* getFramebuffer() { return _framebuffer; }
			inline graphics::Texture* getShadowmapTexture() { return _shadowmap; }
		};

		class DirectionalShadowCaster : public ShadowCaster
		{
		public:
			DirectionalShadowCaster();
			~DirectionalShadowCaster();
			virtual void init(unsigned int shadowmapWidth, unsigned int shadowmapHeight) override;
			virtual void update() override;
		};
	}
}