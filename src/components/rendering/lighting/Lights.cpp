
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
			unsigned int shadowmapWidth,
			unsigned int shadowmapHeight,
			const std::string& name
		) :
			Light(color, ambientColor, name),
			_direction(direction)
		{
			s_directionalLight = this;

			if (shadowmapWidth > 0 && shadowmapHeight > 0)
				_shadowCaster.init(shadowmapWidth, shadowmapHeight);
		}

		DirectionalLight::~DirectionalLight()
		{
			Debug::notify_on_destroy(_name + "(Light)");
		}

		void DirectionalLight::update()
		{
			_shadowCaster.update();
		}

		const size_t DirectionalLight::getSize() const { return sizeof(*this); }
	}
}