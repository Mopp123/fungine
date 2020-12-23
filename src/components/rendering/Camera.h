#pragma once

#include "components/Component.h"
#include "utils/myMathLib/MyMathLib.h"
#include "components/common/Transform.h"

#define CAMERA_DEFAULT_NAME "MainCamera"

namespace fungine
{
	namespace components
	{
		class Camera : public Component
		{
		private:
			float _fov = 70.0f;
			float _aspectRatio = 0.0f;
			float _zNear = 0.0f;
			float _zFar = 0.0f;

			mml::Matrix4 _projectionMatrix;
			mml::Matrix4 _viewMatrix;

			Transform* _entityTransform = nullptr;

			static Camera* s_currentCamera;

		public:

			Camera(const std::string& name = CAMERA_DEFAULT_NAME);
			~Camera();

			void setPerspectiveProjection(float fov, float aspectRatio, float zNear, float zFar);
			void setOrthographicProjection(float left, float right, float top, float bottom, float zNear, float zFar);

			virtual void update() override;

			inline void setProjectionMatrix(const mml::Matrix4& projMat) { _projectionMatrix = projMat; }
			
			inline const float getFov() const { return _fov; }
			inline const float getAspectRatio() const { return _aspectRatio; }
			inline const float getZNear() const { return _zNear; }
			inline const float getZFar() const { return _zFar; }

			inline mml::Matrix4& getViewMatrix() { return _viewMatrix; }
			inline mml::Matrix4& getProjectionMatrix() { return _projectionMatrix; }

			inline static Camera* get_current_camera() { return s_currentCamera; }

			virtual const size_t getSize() const override;
		};
	}
}