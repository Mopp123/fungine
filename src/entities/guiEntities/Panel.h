#pragma once

#include "entities/Entity.h"

namespace fungine
{
	namespace entities
	{
		namespace guiEntityFactory
		{

			std::vector<Entity*> create_panel_entity(int posX, int posY, int width, int height, std::string textElem = "", int textPosX = 0, int textPosY = 0);

		}
	}
}