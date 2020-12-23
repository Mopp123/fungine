
#include "CommonEntityFactory.h"
#include "components/rendering/Camera.h"
#include "components/rendering/lighting/Lights.h"
#include "components/common/Transform.h"


namespace fungine
{
	using namespace components;

	namespace entities
	{
		namespace commonEntityFactory
		{
			Entity* create_entity__Camera(const mml::Vector3& pos, const mml::Quaternion& rot)
			{
				Entity* e = new Entity;
				e->addComponent(std::make_shared<Transform>(pos, rot, mml::Vector3(1, 1, 1)));
				e->addComponent(std::make_shared<Camera>());
				return e;
			}
			
			Entity* create_entity__DirectionalLight(
				const mml::Quaternion& rotation,
				const mml::Vector3& color,
				const mml::Vector3& ambientColor,
				unsigned int shadowmapWidth,
				unsigned int shadowmapHeight
			)
			{
				Entity* e = new Entity;
				std::shared_ptr<Transform> transform = std::make_shared<Transform>(mml::Vector3(-10, 10, 10), rotation, mml::Vector3(1, 1, 1));
				e->addComponent(transform);
				e->addComponent(std::make_shared<DirectionalLight>(color, ambientColor, shadowmapWidth, shadowmapHeight, transform));
				return e;
			}
		}
	}
}