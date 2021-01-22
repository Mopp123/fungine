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

			mml::Vector3	_localPosition;
			mml::Quaternion _localRotation;
			mml::Vector3	_localScale;

			mml::Vector3	_position;
			mml::Quaternion _rotation;
			mml::Vector3	_scale;

			bool _hasChanged = true;

			mml::Matrix4 _transformationMatrix;

		public:

			Transform(const mml::Vector3& pos, const mml::Quaternion& rot, const mml::Vector3& scale);
			~Transform();
			
			const mml::Matrix4& getTransformationMatrix();
			inline float* getTransformationMatrixBuff() { return _transformationMatrix.getRawArr(); }

			mml::Vector3 right()	const;
			mml::Vector3 up()		const;
			mml::Vector3 forward()	const;

			void setPosition(const mml::Vector3& pos);
			void setRotation(const mml::Quaternion& rot);
			void setScale(const mml::Vector3& scale);

			// returns "global and combined transformation"
			const mml::Vector3&		getPosition();
			const mml::Quaternion&	getRotation();
			const mml::Vector3&		getScale();

			inline const mml::Vector3&		getLocalPosition()	const { return _localPosition; }
			inline const mml::Quaternion&	getLocalRotation()	const { return _localRotation; }
			inline const mml::Vector3&		getLocalScale()		const { return _localScale; }

			inline void setParent(Transform* p) { _parent = p; }

			inline bool hasChanged() const { return _hasChanged; }

			virtual const size_t getSize() const override;

		private:

			// Creates the final combined transformation matrix taking possible parent transform into account..
			void createTransformationMatrix();

			void extractFromTransformationMatrix_position(const mml::Matrix4& TM, mml::Vector3& out_pos)	const;
			void extractFromTransformationMatrix_rotation(const mml::Matrix4& TM, mml::Quaternion& out_rot) const;
			void extractFromTransformationMatrix_scale(const mml::Matrix4& TM, mml::Vector3& out_scale)		const;

		};
	}
}