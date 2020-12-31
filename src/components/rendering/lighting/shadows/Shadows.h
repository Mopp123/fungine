#pragma once

#include "utils/myMathLib/MyMathLib.h"
#include "components/common/Transform.h"
#include "graphics/Framebuffer.h"
#include <memory>

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
			graphics::Texture* _shadowmap = nullptr;

			std::shared_ptr<Transform> _transform = nullptr;

			float _maxShadowDistance = 100.0f;

		public:

			ShadowCaster(const std::shared_ptr<Transform>& t);
			virtual ~ShadowCaster() {}
			virtual void init(unsigned int shadowmapWidth, unsigned int shadowmapHeight) = 0;

			virtual void update() = 0;

			inline const mml::Matrix4& getProjectionMatrix() const { return _projectionMatrix; }
			inline const mml::Matrix4& getViewMatrix()const  { return _viewMatrix; }

			inline const graphics::Framebuffer* getFramebuffer() const	{ return _framebuffer; }
			inline const graphics::Texture* getShadowmapTexture() const { return _shadowmap; }
		};

		class DirectionalShadowCaster : public ShadowCaster
		{
		public:
			DirectionalShadowCaster(const std::shared_ptr<Transform>& t);
			~DirectionalShadowCaster();
			virtual void init(unsigned int shadowmapWidth, unsigned int shadowmapHeight) override;
			virtual void update() override;
		};
	}
}