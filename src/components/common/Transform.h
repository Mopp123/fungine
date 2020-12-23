#pragma once

#include "components/Component.h"
#include <vector>
#include "utils/myMathLib/MyMathLib.h"

#define DEFAULT_NAME_TRANSFORM "Transform"

namespace fungine
{
	namespace components
	{
		class Transform : public Component
		{
		private:

			Transform* _parent = nullptr;

			mml::Matrix4 _translationMatrix;
			mml::Matrix4 _rotationMatrix;
			mml::Matrix4 _scaleMatrix;

			mml::Matrix4 _localMatrix;

			mml::Matrix4 _transformationMatrix;

			bool _hasChanged = false;

		public:


			Transform(const mml::Vector3& pos, const mml::Quaternion& rot, const mml::Vector3& scale, bool isStatic = false);
			~Transform();
			virtual void update() override;

			void setPosition(const mml::Vector3& pos);
			void setRotation(const mml::Quaternion& rot);
			void setScale(const mml::Vector3& scale);

			mml::Vector3 getPosition() const;
			mml::Quaternion getRotation() const;
			mml::Vector3 getScale() const;

			const mml::Vector3& getLocalPosition() const;
			const mml::Quaternion& getLocalRotation() const;
			const mml::Vector3& getLocalScale() const;

			mml::Vector3 right() const;
			mml::Vector3 up() const;
			mml::Vector3 forward() const;

			inline mml::Matrix4& getTransformationMatrix() { return _transformationMatrix; }

			inline void setParent(Transform* p) { _parent = p; }

			virtual const size_t getSize() const override;
		};
	}
}