#pragma once

#include "entities/Entity.h"
#include "utils/myMathLib/MyMathLib.h"

namespace fungine
{
	namespace entities
	{
		namespace commonEntityFactory
		{
			Entity* create_entity__Plane2D(const mml::Vector2& pos, const mml::Vector2& scale, unsigned int instanceCount = 1);
		}
	}
}