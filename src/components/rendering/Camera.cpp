
#include "Camera.h"
#include "core/Debug.h"
#include "utils/myMathLib/MyMathLib.h"
#include "entities/Entity.h"

namespace fungine
{
	using namespace entities;

	namespace components
	{

		Camera* Camera::s_currentCamera = nullptr;

		Camera::Camera(const std::string& name) :
			Component(name)
		{
			// *->TEMP just temporarely make current camera the lastly created camera
			s_currentCamera = this;
		}

		Camera::~Camera()
		{
			Debug::notify_on_destroy(_name + "(Camera)");
		}

		void Camera::setPerspectiveProjection(float fov, float aspectRatio, float zNear, float zFar)
		{
			_fov = fov;
			_aspectRatio = aspectRatio;
			_zNear = zNear;
			_zFar = zFar;
			mml::create_perspective_projection_matrix(_projectionMatrix, _fov, _aspectRatio, _zNear, _zFar);
		}
		void Camera::setOrthographicProjection(float left, float right, float top, float bottom, float zNear, float zFar)
		{
			_zNear = zNear;
			_zFar = zFar;
			mml::create_orthographic_projection_matrix(_projectionMatrix, left, right, top, bottom, _zNear, _zFar);
		}

		void Camera::update()
		{
			if(!_entityTransform)
				_entityTransform = _entity->getComponent<Transform>().get(); // *->TEMP : quite disqusting.. may be dangerous?

			mml::create_view_matrix(_viewMatrix, _entityTransform->getPosition(), _entityTransform->getRotation());
		}

		const size_t Camera::getSize() const
		{
			return sizeof(*this);
		}
	}
}