
#include "Shadows.h"
#include "components/rendering/Camera.h"

namespace fungine
{
	using namespace graphics;

	namespace components
	{

		DirectionalShadowCaster::DirectionalShadowCaster() {}
		DirectionalShadowCaster::~DirectionalShadowCaster()
		{
			delete _framebuffer;
		}

		void DirectionalShadowCaster::init(unsigned int shadowmapWidth, unsigned int shadowmapHeight)
		{
			_framebuffer = Framebuffer::create_framebuffer(shadowmapWidth, shadowmapHeight);
			_shadowmap = _framebuffer->createDepthAttachment();
		}

		void DirectionalShadowCaster::update()
		{
			Camera* camera = Camera::get_current_camera();
			_viewMatrix = camera->getViewMatrix();

			mml::create_orthographic_projection_matrix(_projectionMatrix, -1024 * 0.5f, 1024 * 0.5f, 768 * 0.5f, -768 * 0.5f, 0.1f, 800.0f);
		}
	}
}