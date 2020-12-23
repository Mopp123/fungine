
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

		Camera::Camera(const mml::Matrix4& projMat, const std::string& name) :
			Component(name), _projectionMatrix(projMat)
		{
			// *->TEMP just temporarely make current camera the lastly created camera
			s_currentCamera = this;
		}

		Camera::~Camera()
		{
			Debug::notify_on_destroy(_name + "(Camera)");
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