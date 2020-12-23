#pragma once

#include "entities/Entity.h"
#include "utils/myMathLib/MyMathLib.h"

namespace fungine
{
	namespace entities
	{
		namespace commonEntityFactory
		{
			Entity* create_entity__Camera(const mml::Vector3& pos, const mml::Quaternion& rot);
			
			Entity* create_entity__DirectionalLight(
				const mml::Quaternion& rotation, 
				const mml::Vector3& color, 
				const mml::Vector3& ambientColor,
				unsigned int shadowmapWidth = 0,
				unsigned int shadowmapHeight = 0
			);
		}
	}
}