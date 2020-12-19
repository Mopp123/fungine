
#include "Lights.h"
#include "core/Debug.h"

namespace fungine
{
	namespace components
	{

		DirectionalLight* DirectionalLight::s_directionalLight = nullptr;

		DirectionalLight::DirectionalLight(
			const mml::Vector3& direction, 
			const mml::Vector3& color, 
			const mml::Vector3& ambientColor, 
			entities::Entity* entity
		) :
			Light(color, ambientColor, entity), _direction(direction)
		{
			s_directionalLight = this;
		}

		DirectionalLight::~DirectionalLight()
		{
			Debug::notify_on_destroy("DirectionalLight");
		}

		const size_t DirectionalLight::getSize() const { return sizeof(*this); }
	}
}