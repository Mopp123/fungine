#pragma once

#include "components/Component.h"
#include "graphics/Texture.h"
#include "core/window/input/CursorPosEvent.h"
#include "components/common/Transform.h"
#include "components/rendering/renderers/Renderer.h"

#include <memory>

// Length of "_graphicalDataBuffer" which gets sent to the batch's total data buffer when rendering this GUIImage
// color = 4 floats : borderColor = 4 floats : border thickness = 1 float : textureOffset = 2 floats
#define GUIIMAGE__BUFFER_LENGTH__graphicalData 11

namespace fungine
{
	namespace components
	{
		class GUIImage;
		class GUIImageCursorPosEvent : public core::CursorPosEvent
		{
		private:
			entities::Entity* _owner = nullptr;
			GUIImage* _img = nullptr;
			Transform* _transform = nullptr;
			rendering::BatchInstanceData* _imgPropertiesData = nullptr;
			
			mml::Vector4 _originalColor;
			mml::Vector4 _highlightColor;

		public:
			void init(entities::Entity* owner, GUIImage* imgComponent, bool useHighlight = false);
			virtual void callbackFunc(double x, double y) override;
		};


		class GUIImage : public Component
		{
		private:
			static mml::Vector4 s_default_highlightColor;
			static mml::Vector4 s_default_color;
			static mml::Vector4 s_default_borderColor;

		protected:
			graphics::TextureAtlas* _textureAtlas = nullptr; // *NOTE: This component doesn't own this texture, it just has pointer to it
			
			mml::Vector4 _color;
			mml::Vector4 _borderColor;
			int _borderThickness = 0; // measured in pixels

			mml::Vector2 _textureOffset;
			float _graphicalDataBuffer[GUIIMAGE__BUFFER_LENGTH__graphicalData];

			bool _isMouseOver = false;

			GUIImageCursorPosEvent* _cursorPosEvent = nullptr;
			bool _createCursorPosEvent = false;

			friend class GUIImageCursorPosEvent;

		public:
			GUIImage(int borderThickness = 0, bool createCursorPosEvent = false);
			GUIImage(graphics::TextureAtlas* textureAtlas, mml::Vector2 textureOffset = mml::Vector2(0,0));
			virtual ~GUIImage();
			void setColor(const mml::Vector4& color);
			void setBorder(const mml::Vector4& color, int thickness);
			void setTextureOffset(const mml::Vector2& offset);

			virtual void onAttackToEntity(entities::Entity*) override;

			virtual const size_t getSize() const override;

			inline const graphics::TextureAtlas* getTextureAtlas() const { return _textureAtlas; }
			inline const mml::Vector2& getTextureOffset() const { return _textureOffset; }

			inline const mml::Vector4& getColor() const { return _color; }
			inline const mml::Vector4& getBorderColor() const { return _borderColor; }
			inline const int& getBorderThickness() const { return _borderThickness; }

			inline float* getGraphicalDataBuffer() { return _graphicalDataBuffer; }

			inline bool hasCursorPosEvent() const { return _cursorPosEvent != nullptr; }
			inline bool isMouseOver() const { return _isMouseOver; }
		};
	}
}