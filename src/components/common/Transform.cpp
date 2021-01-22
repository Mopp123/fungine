
#include "Transform.h"
#include "entities/Entity.h"
#include "core/Debug.h"

namespace fungine
{
	namespace components
	{

		Transform::Transform(const mml::Vector3& pos, const mml::Quaternion& rot, const mml::Vector3& scale) :
			Component(DEFAULT_NAME_TRANSFORM, true), _localPosition(pos), _localRotation(rot), _localScale(scale)
		{
			createTransformationMatrix();
			_hasChanged = false;
		}

		Transform::~Transform()
		{
			Debug::notify_on_destroy(DEFAULT_NAME_TRANSFORM);
		}

		/*
		const mml::Vector3& Transform::getPosition() const
		{
			return { _transformationMatrix[0 + 3 * 4], _transformationMatrix[1 + 3 * 4], _transformationMatrix[2 + 3 * 4] };
		}*/

		/*const mml::Quaternion& Transform::getRotation() const // *->TEMP OPTIMIZE THE SHIT OUT OF THIS!
		{
			mml::Matrix4 m = _transformationMatrix;
			// Extract translation completely away
			m[0 + 3 * 4] = 0.0f;
			m[1 + 3 * 4] = 0.0f;
			m[2 + 3 * 4] = 0.0f;
			
			// Attempt to negate the scale
			float div_sx = 1.0f / (mml::Vector3(m[0 + 0 * 4], m[1 + 0 * 4], m[2 + 0 * 4]).magnitude());
			float div_sy = 1.0f / (mml::Vector3(m[0 + 1 * 4], m[1 + 1 * 4], m[2 + 1 * 4]).magnitude());
			float div_sz = 1.0f / (mml::Vector3(m[0 + 2 * 4], m[1 + 2 * 4], m[2 + 2 * 4]).magnitude());

			//float sx = mml::Vector3(m[0 + 0 * 4], m[1 + 0 * 4], m[2 + 0 * 4]).magnitude();
			//float sy = mml::Vector3(m[0 + 1 * 4], m[1 + 1 * 4], m[2 + 1 * 4]).magnitude();
			//float sz = mml::Vector3(m[0 + 2 * 4], m[1 + 2 * 4], m[2 + 2 * 4]).magnitude();

			m[0 + 0 * 4] *= div_sx;
			m[1 + 0 * 4] *= div_sx;
			m[2 + 0 * 4] *= div_sx;
			
			m[0 + 1 * 4] *= div_sy;
			m[1 + 1 * 4] *= div_sy;
			m[2 + 1 * 4] *= div_sy;

			m[0 + 2 * 4] *= div_sz;
			m[1 + 2 * 4] *= div_sz;
			m[2 + 2 * 4] *= div_sz;

			// Now we should have just the pure rotation matrix -> convert that to quaternion
			return m.toQuaternion();
		}*/

		/*const mml::Vector3& Transform::getScale() const
		{
			mml::Matrix4 m = _transformationMatrix;
			// Extract translation completely away
			m[0 + 3 * 4] = 0.0f;
			m[1 + 3 * 4] = 0.0f;
			m[2 + 3 * 4] = 0.0f;

			float sx = (mml::Vector3(m[0 + 0 * 4], m[1 + 0 * 4], m[2 + 0 * 4]).magnitude());
			float sy = (mml::Vector3(m[0 + 1 * 4], m[1 + 1 * 4], m[2 + 1 * 4]).magnitude());
			float sz = (mml::Vector3(m[0 + 2 * 4], m[1 + 2 * 4], m[2 + 2 * 4]).magnitude());
			return { sx,sy,sz };
		}*/

		mml::Vector3 Transform::right() const
		{
			return mml::Vector3(
				_transformationMatrix[0 + 0 * 4],
				_transformationMatrix[1 + 0 * 4],
				_transformationMatrix[2 + 0 * 4]
			);
		}
		mml::Vector3 Transform::up() const
		{
			return mml::Vector3(
				_transformationMatrix[0 + 1 * 4],
				_transformationMatrix[1 + 1 * 4],
				_transformationMatrix[2 + 1 * 4]
			);
		}
		mml::Vector3 Transform::forward() const
		{
			return mml::Vector3(
				_transformationMatrix[0 + 2 * 4],
				_transformationMatrix[1 + 2 * 4],
				_transformationMatrix[2 + 2 * 4]
			);
		}

		void Transform::setPosition(const mml::Vector3& pos) 
		{ 
			if (pos != _localPosition) _hasChanged = true;
			_localPosition = pos; 
		}
		void Transform::setRotation(const mml::Quaternion& rot)
		{
			if (rot != _localRotation) _hasChanged = true;
			_localRotation = rot;
		}
		void Transform::setScale(const mml::Vector3& scale) 
		{ 
			if (scale != _localScale) _hasChanged = true;
			_localScale = scale;
		}

		const mml::Vector3& Transform::getPosition()
		{
			if (_hasChanged)
			{
				createTransformationMatrix();
				_hasChanged = false;
			}
			extractFromTransformationMatrix_position(_transformationMatrix, _position);
			return _position;
		}
		const mml::Quaternion& Transform::getRotation()
		{
			if (_hasChanged)
			{
				createTransformationMatrix();
				_hasChanged = false;
			}
			extractFromTransformationMatrix_rotation(_transformationMatrix, _rotation);
			return _rotation;
		}
		const mml::Vector3& Transform::getScale()
		{
			if (_hasChanged)
			{
				createTransformationMatrix();
				_hasChanged = false;
			}
			extractFromTransformationMatrix_scale(_transformationMatrix, _scale);
			return _scale;
		}



		const mml::Matrix4& Transform::getTransformationMatrix()
		{
			if (_hasChanged)
			{
				createTransformationMatrix();
				_hasChanged = false;
			}
			return _transformationMatrix;
		}

		// Creates the final combined transformation matrix from local matrix and parent matrix..
		void Transform::createTransformationMatrix()
		{
			mml::Matrix4 localMatrix(1.0f);
			mml::translate_matrix(localMatrix, _localPosition);
			mml::rotate_matrix(localMatrix, _localRotation);
			mml::scale_matrix(localMatrix, _localScale);

			if (_parent)
				_transformationMatrix = _parent->getTransformationMatrix() * localMatrix;
			else
				_transformationMatrix = localMatrix;
		}

		void Transform::extractFromTransformationMatrix_position(const mml::Matrix4& TM, mml::Vector3& out_pos) const
		{
			out_pos.x = TM[0 + 3 * 4];
			out_pos.y = TM[1 + 3 * 4];
			out_pos.z = TM[2 + 3 * 4];
		}
		void Transform::extractFromTransformationMatrix_rotation(const mml::Matrix4& TM, mml::Quaternion& out_rot) const
		{
			mml::Matrix4 m = TM;
			// Extract translation completely away
			m[0 + 3 * 4] = 0.0f;
			m[1 + 3 * 4] = 0.0f;
			m[2 + 3 * 4] = 0.0f;

			// Attempt to negate the scale
			float div_sx = 1.0f / (mml::Vector3(m[0 + 0 * 4], m[1 + 0 * 4], m[2 + 0 * 4]).magnitude());
			float div_sy = 1.0f / (mml::Vector3(m[0 + 1 * 4], m[1 + 1 * 4], m[2 + 1 * 4]).magnitude());
			float div_sz = 1.0f / (mml::Vector3(m[0 + 2 * 4], m[1 + 2 * 4], m[2 + 2 * 4]).magnitude());

			//float sx = mml::Vector3(m[0 + 0 * 4], m[1 + 0 * 4], m[2 + 0 * 4]).magnitude();
			//float sy = mml::Vector3(m[0 + 1 * 4], m[1 + 1 * 4], m[2 + 1 * 4]).magnitude();
			//float sz = mml::Vector3(m[0 + 2 * 4], m[1 + 2 * 4], m[2 + 2 * 4]).magnitude();

			m[0 + 0 * 4] *= div_sx;
			m[1 + 0 * 4] *= div_sx;
			m[2 + 0 * 4] *= div_sx;

			m[0 + 1 * 4] *= div_sy;
			m[1 + 1 * 4] *= div_sy;
			m[2 + 1 * 4] *= div_sy;

			m[0 + 2 * 4] *= div_sz;
			m[1 + 2 * 4] *= div_sz;
			m[2 + 2 * 4] *= div_sz;

			// Now we should have just the pure rotation matrix -> convert that to quaternion
			out_rot = m.toQuaternion();
		}
		void Transform::extractFromTransformationMatrix_scale(const mml::Matrix4& TM, mml::Vector3& out_scale) const
		{
			mml::Matrix4 m = TM;
			// Extract translation completely away
			m[0 + 3 * 4] = 0.0f;
			m[1 + 3 * 4] = 0.0f;
			m[2 + 3 * 4] = 0.0f;

			out_scale.x = (mml::Vector3(m[0 + 0 * 4], m[1 + 0 * 4], m[2 + 0 * 4]).magnitude());
			out_scale.y = (mml::Vector3(m[0 + 1 * 4], m[1 + 1 * 4], m[2 + 1 * 4]).magnitude());
			out_scale.z = (mml::Vector3(m[0 + 2 * 4], m[1 + 2 * 4], m[2 + 2 * 4]).magnitude());
		}


		const size_t Transform::getSize() const
		{
			return sizeof(*this);
		}

	}
}