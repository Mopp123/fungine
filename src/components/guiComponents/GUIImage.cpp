
#include "GUIImage.h"
#include "core/resourceManaging/ResourceManager.h"
#include "entities/Entity.h"

#include "core/window/input/InputHandler.h"

#include "core/Debug.h"

namespace fungine
{
	namespace components
	{

		void GUIImageCursorPosEvent::init(entities::Entity* owner, GUIImage* imgComponent, bool useHighlight)
		{
			_owner = owner;
			_img = imgComponent;
			std::shared_ptr<Transform> transformComponent = _owner->getComponent<Transform>();
#ifdef DEBUG__MODE_FULL
			if (!transformComponent)
			{
				Debug::log(
					"Location: GUIImageCursorPosEvent::init(entities::Entity* owner, GUIImage* imgComponent, bool useHighlight, mml::Vector4 highlightColor)\n"
					"Attempted to add cursorPosEvent to GUIImage, but failed to find the GUIImage's entity's 'Transform' -component",
					DEBUG__ERROR_LEVEL__ERROR
				);
				return;
			}
#endif

			_transform = transformComponent.get(); // A little dangerous stuff...
			
			_originalColor = _img->getColor();
			_highlightColor = GUIImage::s_default_highlightColor;
		}
		void GUIImageCursorPosEvent::callbackFunc(double x, double y)
		{
#ifdef DEBUG__MODE_FULL
			if (!_transform)
				return;
#endif
			// Get handle to the per instance data of this image's properties (second per instance data component)
			if (!_imgPropertiesData)
			{
				std::vector<std::shared_ptr<rendering::BatchInstanceData>> instancedDataComponents = _owner->getComponents<rendering::BatchInstanceData>();
				if (!instancedDataComponents.empty())
				{
					if (instancedDataComponents.size() > 1)
						_imgPropertiesData = instancedDataComponents[1].get();
				}
				return;
			}

			const mml::Vector3& pos = _transform->getPosition();
			const mml::Vector3& scale = _transform->getScale();
			float fx = (float)x;
			float fy = (float)y;

			if (fx >= pos.x && fx <= pos.x + scale.x && fy >= pos.y && fy <= pos.y + scale.y)
			{
				if(!_img->_isMouseOver)
					_originalColor = _img->getColor();
				
				_img->_isMouseOver = true;
				_img->setColor(_highlightColor);
			}
			else
			{
				_img->_isMouseOver = false;
				_img->setColor(_originalColor);
			}
			_imgPropertiesData->update();
		}


		mml::Vector4 GUIImage::s_default_color = mml::Vector4(0.15f, 0.15f, 0.15f, 1.0f);
		mml::Vector4 GUIImage::s_default_borderColor = mml::Vector4(0.25f, 0.25f, 0.25f, 1);
		mml::Vector4 GUIImage::s_default_highlightColor = mml::Vector4(0.175f, 0.175f, 0.175f, 1.0f);

		GUIImage::GUIImage(int borderThickness, bool createCursorPosEvent) :
			Component("GUIImage", true), _textureAtlas(core::ResourceManager::get_default_textureAtlas2D()), _createCursorPosEvent(createCursorPosEvent)
		{
			setColor(s_default_color);
			setBorder(s_default_borderColor, borderThickness);
		}

		GUIImage::GUIImage(graphics::TextureAtlas* textureAtlas, mml::Vector2 textureOffset) :
			Component("GUIImage", true), _textureAtlas(textureAtlas)
		{
			setColor(mml::Vector4(1, 1, 1, 1));
			setBorder({ 1,1,1,1 }, 1.0f);
		}

		GUIImage::~GUIImage()
		{
			if (_cursorPosEvent)
				delete _cursorPosEvent;

			Debug::notify_on_destroy(_name);
		}

		void GUIImage::onAttackToEntity(entities::Entity* owner)
		{
			if (_createCursorPosEvent)
			{
				_cursorPosEvent = new GUIImageCursorPosEvent;
				_cursorPosEvent->init(owner, this, true);
				core::InputHandler::add_cursor_pos_event(_cursorPosEvent);
			}
		}

		void GUIImage::setColor(const mml::Vector4& color)
		{
			_color = color;
			_graphicalDataBuffer[0] = _color.x;
			_graphicalDataBuffer[1] = _color.y;
			_graphicalDataBuffer[2] = _color.z;
			_graphicalDataBuffer[3] = _color.w;
		}

		void GUIImage::setBorder(const mml::Vector4& color, int thickness)
		{
			_borderColor = color;
			_borderThickness = thickness;

			_graphicalDataBuffer[4] = _borderColor.x;
			_graphicalDataBuffer[5] = _borderColor.y;
			_graphicalDataBuffer[6] = _borderColor.z;
			_graphicalDataBuffer[7] = _borderColor.w;
		
			_graphicalDataBuffer[8] = _borderThickness;
		}
		
		void GUIImage::setTextureOffset(const mml::Vector2& offset)
		{
			_textureOffset = offset;
			_graphicalDataBuffer[9] = _textureOffset.x;
			_graphicalDataBuffer[10] = _textureOffset.y;
		}


		const size_t GUIImage::getSize() const
		{
			return sizeof(*this);
		}
	}
}