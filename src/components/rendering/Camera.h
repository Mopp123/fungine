#pragma once

#include "components/Component.h"
#include "utils/myMathLib/MyMathLib.h"
#include "components/common/Transform.h"

namespace fungine
{
	namespace components
	{
		class Camera : public Component
		{
		private:
			mml::Matrix4 _projectionMatrix;
			mml::Matrix4 _viewMatrix;

			Transform* _entityTransform = nullptr;

			static Camera* s_currentCamera;

		public:

			Camera(const mml::Matrix4& projMat, entities::Entity* entity = nullptr);
			~Camera();

			virtual void update() override;

			inline mml::Matrix4& getViewMatrix() { return _viewMatrix; }

			inline void setProjectionMatrix(const mml::Matrix4& projMat) { _projectionMatrix = projMat; }
			inline mml::Matrix4& getProjectionMatrix() { return _projectionMatrix; }

			inline static Camera* get_current_camera() { return s_currentCamera; }

			virtual const size_t getSize() const override;
		};
	}
}