
#include "Lights.h"
#include "entities/Entity.h"
#include "components/common/Transform.h"
#include "core/Debug.h"
#include <memory>

namespace fungine
{
	using namespace entities;

	namespace components
	{

		DirectionalLight* DirectionalLight::s_directionalLight = nullptr;

		DirectionalLight::DirectionalLight(
			const mml::Vector3& color, 
			const mml::Vector3& ambientColor,
			unsigned int shadowmapWidth,
			unsigned int shadowmapHeight,
			const std::shared_ptr<Transform>& entityTransform,
			const std::string& name
		) :
			Light(color, ambientColor, name),
			_shadowCaster(entityTransform)
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

		const mml::Vector3& DirectionalLight::getDirection() const
		{
			return _entity->getComponent<Transform>()->forward();
		}

		const size_t DirectionalLight::getSize() const { return sizeof(*this); }
	}
}