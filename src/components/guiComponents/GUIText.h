#pragma once

#include "components/Component.h"
#include "utils/myMathLib/MyMathLib.h"
#include "utils/fontUtils/Font.h"

// Length of "_graphicalDataBuffer" which gets sent to the batch's total data buffer when rendering this GUIText
#define TEXT__BUFFER_LENGTH__graphicalData 4

namespace fungine
{
	namespace components
	{

		class GUIText : public Component
		{
		private:

			std::string _str;
			float _scalingFactor = 1.0f;
			bool _hasChanged = true;

			Font* _font = nullptr;

		public:

			GUIText(std::string initStr, Font* font, float scalingFactor = 1.0f);
			GUIText(std::string initStr, float scalingFactor = 1.0f);
			~GUIText();
			void setText(const std::string& str);

			virtual const size_t getSize() const override;
		
			inline const std::string& getText() const { return _str; }
			inline float getScalingFactor() const { return _scalingFactor; }
			inline bool hasChanged() const { return _hasChanged; }

			inline Font* getFont() const { return _font; }
		};
	}
}