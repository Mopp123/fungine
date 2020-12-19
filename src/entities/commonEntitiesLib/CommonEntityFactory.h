#pragma once

#include "entities/Entity.h"
#include "utils/myMathLib/MyMathLib.h"

namespace fungine
{
	namespace entities
	{
		namespace commonEntityFactory
		{
			Entity* create_entity__Camera(const mml::Vector3& pos, const mml::Quaternion& rot, const mml::Matrix4& projMat);
			Entity* create_entity__DirectionalLight(const mml::Vector3& direction, const mml::Vector3& color, const mml::Vector3& ambientColor);

		}
	}
}