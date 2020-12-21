
#include "Transform.h"
#include "entities/Entity.h"
#include "core/Debug.h"

namespace fungine
{
	namespace components
	{

		Transform::Transform(const mml::Vector3& pos, const mml::Quaternion& rot, const mml::Vector3& scale, entities::Entity* entity) : 
			Component(entity)
		{
			_translationMatrix.setIdentity();
			_rotationMatrix.setIdentity();
			_scaleMatrix.setIdentity();

			_localMatrix.setIdentity();
			_transformationMatrix.setIdentity();

			setPosition(pos);
			setRotation(rot);
			setScale(scale);

			update();
		}

		Transform::~Transform()
		{
			Debug::notify_on_destroy("Transform");
		}

		void Transform::update()
		{
			_localMatrix = _translationMatrix * _rotationMatrix * _scaleMatrix;

			if (_parent)
				_transformationMatrix = _parent->getTransformationMatrix() * _localMatrix;
			else
				_transformationMatrix = _localMatrix;
		}

		void Transform::setPosition(const mml::Vector3& pos)
		{
			_translationMatrix.setIdentity();
			mml::translate_matrix(_translationMatrix, pos);
		}

		void Transform::setRotation(const mml::Quaternion& rot)
		{
			_rotationMatrix = rot.toRotationMatrix();
		}
		void Transform::setScale(const mml::Vector3& scale)
		{
			_scaleMatrix.setIdentity();
			mml::scale_matrix(_scaleMatrix, scale);
		}

		mml::Vector3 Transform::getPosition() const
		{
			return { _transformationMatrix[0 + 3 * 4],_transformationMatrix[1 + 3 * 4] ,_transformationMatrix[2 + 3 * 4] };
		}
		mml::Quaternion Transform::getRotation() const // *->TEMP OPTIMIZE THE SHIT OUT OF THIS!
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
		}
		mml::Vector3 Transform::getScale() const
		{
			mml::Matrix4 m = _transformationMatrix;
			// Extract translation completely away
			m[0 + 3 * 4] = 0.0f;
			m[1 + 3 * 4] = 0.0f;
			m[2 + 3 * 4] = 0.0f;

			// Attempt to negate the scale
			float sx = (mml::Vector3(m[0 + 0 * 4], m[1 + 0 * 4], m[2 + 0 * 4]).magnitude());
			float sy = (mml::Vector3(m[0 + 1 * 4], m[1 + 1 * 4], m[2 + 1 * 4]).magnitude());
			float sz = (mml::Vector3(m[0 + 2 * 4], m[1 + 2 * 4], m[2 + 2 * 4]).magnitude());
			return { sx,sy,sz };
		}

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


		const size_t Transform::getSize() const
		{
			return sizeof(*this);
		}

	}
}