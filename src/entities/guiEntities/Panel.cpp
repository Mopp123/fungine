
#include "Panel.h"
#include "components/guiComponents/GUIImage.h"
#include "components/guiComponents/GUIPanel.h"

#include "components/rendering/renderers/GUIRenderer.h"
#include "components/rendering/renderers/GUITextRenderer.h"


#define GUI_PANEL__TITLE_BAR_HEIGHT 12

namespace fungine
{
	using namespace components;

	namespace entities
	{
		namespace guiEntityFactory
		{
			std::vector<Entity*> create_panel_entity(int posX, int posY, int width, int height, std::string textElem, int textPosX, int textPosY)
			{
				float fPosX = (float)posX;
				float fPosY = (float)posY;
				float fWidth = (float)width;
				float fHeight = (float)height;

				std::vector<Entity*> entities;

				Entity* masterEntity = new Entity;
				std::shared_ptr<Transform> masterTransform = std::make_shared<Transform>(mml::Vector3(fPosX, fPosY, -1), mml::Quaternion({ 0,1,0 }, 0.0f), mml::Vector3(fWidth, fHeight, 1.0f));
				masterEntity->addComponent(masterTransform);

				Entity* backgroundEntity = new Entity;
				std::shared_ptr<Transform> backgroundTransform = std::make_shared<Transform>(mml::Vector3(0, 0, 0), mml::Quaternion({ 0,1,0 }, 0.0f), mml::Vector3(1.0f, 1.0f, 1.0f));
				backgroundTransform->setParent(masterTransform.get());
				
				std::shared_ptr<GUIImage> backgroundImg = std::make_shared<GUIImage>(1, true);
				backgroundEntity->addComponent(backgroundTransform);
				backgroundEntity->addComponent(backgroundImg);

				entities.push_back(masterEntity);
				entities.push_back(backgroundEntity);

				// Add text if we want to..
				if (!textElem.empty())
				{
					Entity* textEntity = new Entity;
					std::shared_ptr<Transform> textTransform = std::make_shared<Transform>(mml::Vector3(0.5f, 0.0f, 1), mml::Quaternion({ 0,1,0 }, 0.0f), mml::Vector3(1.0f, 1.0f, 1.0f));
					textTransform->setParent(masterTransform.get());

					std::shared_ptr<GUIText> textComponent = std::make_shared<GUIText>(textElem, 1.0f);
					textEntity->addComponent(textTransform);
					textEntity->addComponent(textComponent);

					entities.push_back(textEntity);
				}
				// Create title bar entity if we want...
				/*if (!textElem.empty())
				{
					// Title bar height in pixels
					float titlebarHeight = (float)GUI_PANEL__TITLE_BAR_HEIGHT / fHeight;

					Entity* titleBarEntity = new Entity;
					std::shared_ptr<Transform> titleBarTransform = std::make_shared<Transform>(mml::Vector3(0, 0, 1), mml::Quaternion({ 0,1,0 }, 0.0f), mml::Vector3(1.0f, titlebarHeight, 1.0f));
					titleBarTransform->setParent(masterTransform.get());

					std::shared_ptr<GUIImage> titleBarImg = std::make_shared<GUIImage>(1, true);
					titleBarImg->setBorder({ 0,0,1,1 }, 1);
					titleBarEntity->addComponent(titleBarTransform);
					titleBarEntity->addComponent(titleBarImg);

					entities.push_back(titleBarEntity);
				}*/

				return entities;
			}
		}
	}
}