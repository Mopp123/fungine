
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
			Entity* create_entity__Camera(const mml::Vector3& pos, const mml::Quaternion& rot, const mml::Matrix4& projMat)
			{
				Entity* e = new Entity;
				e->addComponent(std::make_shared<Transform>(pos, rot, mml::Vector3(1, 1, 1)));
				e->addComponent(std::make_shared<Camera>(projMat));
				return e;
			}
			
			Entity* create_entity__DirectionalLight(const mml::Vector3& direction, const mml::Vector3& color, const mml::Vector3& ambientColor)
			{
				Entity* e = new Entity;
				e->addComponent(std::make_shared<Transform>(mml::Vector3(-10, 10, 10), mml::Quaternion({0,1,0}, 0), mml::Vector3(1, 1, 1)));
				e->addComponent(std::make_shared<DirectionalLight>(direction, color, ambientColor));
				return e;
			}
		}
	}
}