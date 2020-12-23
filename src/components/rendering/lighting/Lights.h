#pragma once

#include "components/Component.h"
#include "shadows/Shadows.h"
#include "utils/myMathLib/MyMathLib.h"

#define LIGHT_DEFAULT_NAME "Light"
#define DIRECTIONAL_LIGHT_DEFAULT_NAME "DirectionalLight"

namespace fungine
{
	namespace components
	{
		class Light : public Component
		{
		private:
			mml::Vector3 _color = mml::Vector3(1, 1, 1);
			mml::Vector3 _ambientColor = mml::Vector3(0, 0, 0);

		public:
			Light(const mml::Vector3& color, const mml::Vector3& ambientColor, const std::string& name = LIGHT_DEFAULT_NAME) : 
				Component(name), _color(color), _ambientColor(ambientColor)
			{}
			virtual ~Light() {}

			inline void setColor(const mml::Vector3& color)			{ _color = color; }
			inline void setAmbientColor(const mml::Vector3& color)	{ _ambientColor = color; }
		
			inline const mml::Vector3& getColor() const			{ return _color; }
			inline const mml::Vector3& getAmbientColor() const	{ return _ambientColor; }
		};


		class DirectionalLight : public Light
		{
		private:
			mml::Vector3 _direction = mml::Vector3(0, -1, -1);

			DirectionalShadowCaster _shadowCaster;

			// *->TEMP
			static DirectionalLight* s_directionalLight;

		public:
			DirectionalLight(
				const mml::Vector3& direction,
				const mml::Vector3& color,
				const mml::Vector3& ambientColor,
				unsigned int shadowmapWidth, 
				unsigned int shadowmapHeight,
				const std::string& name = DIRECTIONAL_LIGHT_DEFAULT_NAME
			);
			~DirectionalLight();

			virtual void update() override;

			inline void setDirection(const mml::Vector3& dir) { _direction = dir; }
			inline const mml::Vector3& getDirection() const { return _direction; }
			
			inline ShadowCaster& getShadowCaster() { return _shadowCaster; }

			inline static DirectionalLight* get_directional_light() { return s_directionalLight; }

			const size_t getSize() const override;
		};
	}
}