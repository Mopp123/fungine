#pragma once

#include "entities/Entity.h"
#include "utils/myMathLib/MyMathLib.h"

namespace fungine
{
	namespace entities
	{
		namespace commonEntityFactory
		{
			Entity* create_entity__Plane(const mml::Vector3& pos, const mml::Quaternion& rot, const mml::Vector3& scale);

		}
	}
}