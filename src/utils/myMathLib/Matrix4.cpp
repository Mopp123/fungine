
#include "Matrix4.h"
#include <string>
#include <cmath>

namespace mml
{
	Matrix4::Matrix4()
	{
		memset(_m, 0, sizeof(float) * 16);
	}
	Matrix4::Matrix4(float d)
	{
		memset(_m, 0, sizeof(float) * 16);
		setDiag(1.0f);
	}
	// *Note: You can give the array in nice looking ordering(for example in the inputted array: 1,0 means first row, second column)
	// We transpose this ordering in this constructor.
	Matrix4::Matrix4(float* arr)
	{
		for (int y = 0; y < 4; ++y)
		{
			for (int x = 0; x < 4; ++x)
				_m[y + x * 4] = arr[x + y * 4];
		}
	}
	Matrix4::Matrix4(const Matrix4& mat)
	{
		memcpy(_m, mat._m, sizeof(float) * 16);
	}
	
	void Matrix4::setDiag(float val)
	{
		for (int i = 0; i < 4; ++i) 
			_m[i + i * 4] = val;
	}
	void Matrix4::setIdentity()
	{
		memset(_m, 0, sizeof(float) * 16);
		setDiag(1.0f);
	}
	void Matrix4::transpose()
	{
		Matrix4 temp = *this;
		for (int y = 0; y < 4; ++y)
		{
			for (int x = 0; x < 4; ++x)
			{
				_m[y + x * 4] = temp[x + y * 4];
			}
		}
	}

	//	*Found from: https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
	//		comment on the site about this :
	//			"This was lifted from MESA implementation of the GLU library."
	void Matrix4::inverse()
	{
		mml::Matrix4 inverseMatrix;

		inverseMatrix._m[0] = _m[5] * _m[10] * _m[15] -
			_m[5] * _m[11] * _m[14] -
			_m[9] * _m[6] * _m[15] +
			_m[9] * _m[7] * _m[14] +
			_m[13] * _m[6] * _m[11] -
			_m[13] * _m[7] * _m[10];

		inverseMatrix._m[4] = -_m[4] * _m[10] * _m[15] +
			_m[4] * _m[11] * _m[14] +
			_m[8] * _m[6] * _m[15] -
			_m[8] * _m[7] * _m[14] -
			_m[12] * _m[6] * _m[11] +
			_m[12] * _m[7] * _m[10];

		inverseMatrix._m[8] = _m[4] * _m[9] * _m[15] -
			_m[4] * _m[11] * _m[13] -
			_m[8] * _m[5] * _m[15] +
			_m[8] * _m[7] * _m[13] +
			_m[12] * _m[5] * _m[11] -
			_m[12] * _m[7] * _m[9];

		inverseMatrix._m[12] = -_m[4] * _m[9] * _m[14] +
			_m[4] * _m[10] * _m[13] +
			_m[8] * _m[5] * _m[14] -
			_m[8] * _m[6] * _m[13] -
			_m[12] * _m[5] * _m[10] +
			_m[12] * _m[6] * _m[9];

		inverseMatrix._m[1] = -_m[1] * _m[10] * _m[15] +
			_m[1] * _m[11] * _m[14] +
			_m[9] * _m[2] * _m[15] -
			_m[9] * _m[3] * _m[14] -
			_m[13] * _m[2] * _m[11] +
			_m[13] * _m[3] * _m[10];

		inverseMatrix._m[5] = _m[0] * _m[10] * _m[15] -
			_m[0] * _m[11] * _m[14] -
			_m[8] * _m[2] * _m[15] +
			_m[8] * _m[3] * _m[14] +
			_m[12] * _m[2] * _m[11] -
			_m[12] * _m[3] * _m[10];

		inverseMatrix._m[9] = -_m[0] * _m[9] * _m[15] +
			_m[0] * _m[11] * _m[13] +
			_m[8] * _m[1] * _m[15] -
			_m[8] * _m[3] * _m[13] -
			_m[12] * _m[1] * _m[11] +
			_m[12] * _m[3] * _m[9];

		inverseMatrix._m[13] = _m[0] * _m[9] * _m[14] -
			_m[0] * _m[10] * _m[13] -
			_m[8] * _m[1] * _m[14] +
			_m[8] * _m[2] * _m[13] +
			_m[12] * _m[1] * _m[10] -
			_m[12] * _m[2] * _m[9];

		inverseMatrix._m[2] = _m[1] * _m[6] * _m[15] -
			_m[1] * _m[7] * _m[14] -
			_m[5] * _m[2] * _m[15] +
			_m[5] * _m[3] * _m[14] +
			_m[13] * _m[2] * _m[7] -
			_m[13] * _m[3] * _m[6];

		inverseMatrix._m[6] = -_m[0] * _m[6] * _m[15] +
			_m[0] * _m[7] * _m[14] +
			_m[4] * _m[2] * _m[15] -
			_m[4] * _m[3] * _m[14] -
			_m[12] * _m[2] * _m[7] +
			_m[12] * _m[3] * _m[6];

		inverseMatrix._m[10] = _m[0] * _m[5] * _m[15] -
			_m[0] * _m[7] * _m[13] -
			_m[4] * _m[1] * _m[15] +
			_m[4] * _m[3] * _m[13] +
			_m[12] * _m[1] * _m[7] -
			_m[12] * _m[3] * _m[5];

		inverseMatrix._m[14] = -_m[0] * _m[5] * _m[14] +
			_m[0] * _m[6] * _m[13] +
			_m[4] * _m[1] * _m[14] -
			_m[4] * _m[2] * _m[13] -
			_m[12] * _m[1] * _m[6] +
			_m[12] * _m[2] * _m[5];

		inverseMatrix._m[3] = -_m[1] * _m[6] * _m[11] +
			_m[1] * _m[7] * _m[10] +
			_m[5] * _m[2] * _m[11] -
			_m[5] * _m[3] * _m[10] -
			_m[9] * _m[2] * _m[7] +
			_m[9] * _m[3] * _m[6];

		inverseMatrix._m[7] = _m[0] * _m[6] * _m[11] -
			_m[0] * _m[7] * _m[10] -
			_m[4] * _m[2] * _m[11] +
			_m[4] * _m[3] * _m[10] +
			_m[8] * _m[2] * _m[7] -
			_m[8] * _m[3] * _m[6];

		inverseMatrix._m[11] = -_m[0] * _m[5] * _m[11] +
			_m[0] * _m[7] * _m[9] +
			_m[4] * _m[1] * _m[11] -
			_m[4] * _m[3] * _m[9] -
			_m[8] * _m[1] * _m[7] +
			_m[8] * _m[3] * _m[5];

		inverseMatrix._m[15] = _m[0] * _m[5] * _m[10] -
			_m[0] * _m[6] * _m[9] -
			_m[4] * _m[1] * _m[10] +
			_m[4] * _m[2] * _m[9] +
			_m[8] * _m[1] * _m[6] -
			_m[8] * _m[2] * _m[5];


		float determinant = _m[0] * inverseMatrix._m[0] + _m[1] * inverseMatrix._m[4] + _m[2] * inverseMatrix._m[8] + _m[3] * inverseMatrix._m[12];

		*this = inverseMatrix; // THIS MAY NOT WORK! <- wtf is going on here!?!? don't remember at all..

		if (determinant == 0)
			return;

		for (int i = 0; i < 16; ++i)
			_m[i] *= (1.0f / determinant);
	}


	// Converts PURE rotation matrix into quaternion
	// *Found from: https://math.stackexchange.com/questions/893984/conversion-of-rotation-matrix-to-quaternion
	//		*answerer: SRH
	Quaternion Matrix4::toQuaternion()
	{
		float t = 0.0f; // matrix's trace?
		Quaternion quaternion;
		if (_m[2 + 2 * 4] < 0) {
			if (_m[0 + 0 * 4] > _m[1 + 1 * 4]) {
				t = 1 + _m[0 + 0 * 4] - _m[1 + 1 * 4] - _m[2 + 2 * 4];
				quaternion = Quaternion(t, _m[0 + 1 * 4] + _m[1 + 0 * 4], _m[2 + 0 * 4] + _m[0 + 2 * 4], _m[1 + 2 * 4] - _m[2 + 1 * 4]);
			}
			else {
				t = 1 - _m[0 + 0 * 4] + _m[1 + 1 * 4] - _m[2 + 2 * 4];
				quaternion = Quaternion(_m[0 + 1 * 4] + _m[1 + 0 * 4], t, _m[1 + 2 * 4] + _m[2 + 1 * 4], _m[2 + 0 * 4] - _m[0 + 2 * 4]);
			}
		}
		else {
			if (_m[0 + 0 * 4] < -_m[1 + 1 * 4]) {
				t = 1 - _m[0 + 0 * 4] - _m[1 + 1 * 4] + _m[2 + 2 * 4];
				quaternion = Quaternion(_m[2 + 0 * 4] + _m[0 + 2 * 4], _m[1 + 2 * 4] + _m[2 + 1 * 4], t, _m[0 + 1 * 4] - _m[1 + 0 * 4]);
			}
			else {
				t = 1 + _m[0 + 0 * 4] + _m[1 + 1 * 4] + _m[2 + 2 * 4];
				quaternion = Quaternion(_m[1 + 2 * 4] - _m[2 + 1 * 4], _m[2 + 0 * 4] - _m[0 + 2 * 4], _m[0 + 1 * 4] - _m[1 + 0 * 4], t);
			}
		}
		float half_sqrt_t = 0.5f / std::sqrtf(t);
		return quaternion * half_sqrt_t;
	}

	void Matrix4::print() const
	{
		for (int y = 0; y < 4; ++y)
		{
			printf(
				"[%.2f, %.2f, %.2f, %.2f]\n", 
				_m[y + 0 * 4], 
				_m[y + 1 * 4], 
				_m[y + 2 * 4], 
				_m[y + 3 * 4]
			);
		}
	}
	float& Matrix4::operator[](int i)
	{
		return _m[i];
	}
	const float& Matrix4::operator[](int i) const // I have no idea is this kind of duplication legal??
	{
		return _m[i];
	}

	Matrix4 operator*(const Matrix4& left, const Matrix4& right)
	{
		Matrix4 result;

		for (int y = 0; y < 4; ++y)
		{
			for (int x = 0; x < 4; ++x)
				result[y + x * 4] =
				left[y + 0 * 4] * right[0 + x * 4] +
				left[y + 1 * 4] * right[1 + x * 4] +
				left[y + 2 * 4] * right[2 + x * 4] +
				left[y + 3 * 4] * right[3 + x * 4];
		}

		return result;
	}
	Vector4 operator*(const Matrix4& left, const Vector4& right)
	{
		Vector4 result;

		result.x = left[0 + 0 * 4] * right.x + left[0 + 1 * 4] * right.y + left[0 + 2 * 4] * right.z + left[0 + 3 * 4] * right.w;
		result.y = left[1 + 0 * 4] * right.x + left[1 + 1 * 4] * right.y + left[1 + 2 * 4] * right.z + left[1 + 3 * 4] * right.w;
		result.z = left[2 + 0 * 4] * right.x + left[2 + 1 * 4] * right.y + left[2 + 2 * 4] * right.z + left[2 + 3 * 4] * right.w;
		result.w = left[3 + 0 * 4] * right.x + left[3 + 1 * 4] * right.y + left[3 + 2 * 4] * right.z + left[3 + 3 * 4] * right.w;

		return result;
	}
	Matrix4 operator*(const Matrix4& left, float val)
	{
		return Matrix4(1.0f);
	}
	bool operator==(const Matrix4& left, const Matrix4& right)
	{
		return memcmp(left._m, right._m, sizeof(float) * 16) == 0;
	}
	bool operator!=(const Matrix4& left, const Matrix4& right)
	{
		return memcmp(left._m, right._m, sizeof(float) * 16) != 0;
	}

	// All specific matrix creation functions---------->
	void create_orthographic_projection_matrix(
		Matrix4& outMatrix,
		float left, float right,
		float top, float bottom,
		float zNear, float zFar
	)
	{
		outMatrix.setIdentity();

		outMatrix[0] = 2.0f / (right - left);
		outMatrix[1 + 1 * 4] = 2.0f / (top - bottom);
		outMatrix[2 + 2 * 4] = -2.0f / (zFar - zNear);
		outMatrix[3 + 3 * 4] = 1.0f;
		outMatrix[0 + 3 * 4] = -((right + left) / (right - left));
		outMatrix[1 + 3 * 4] = -((top + bottom) / (top - bottom));
		outMatrix[2 + 3 * 4] = -((zFar + zNear) / (zFar - zNear));
	}

	
	void create_perspective_projection_matrix(
		Matrix4& outMatrix, 
		float fov, 
		float aspectRatio, 
		float zNear, 
		float zFar
	)
	{
		outMatrix.setIdentity();
		outMatrix[0 + 0 * 4] = 1.0f / (aspectRatio * std::tan(fov / 2.0f));
		outMatrix[1 + 1 * 4] = 1.0f / (std::tan(fov / 2.0f));
		outMatrix[2 + 2 * 4] = -((zFar + zNear) / (zFar - zNear));
		outMatrix[2 + 3 * 4] = -((2.0f * zFar * zNear) / (zFar - zNear));
		outMatrix[3 + 2 * 4] = -1.0f;
		outMatrix[3 + 3 * 4] = 0.0f;
	}

	void translate_matrix(Matrix4& m, const Vector3& position)
	{
		Matrix4 translationMatrix(1.0f);
		translationMatrix[0 + 3 * 4] = position.x;
		translationMatrix[1 + 3 * 4] = position.y;
		translationMatrix[2 + 3 * 4] = position.z;
		m = m * translationMatrix;
	}
	void rotate_matrix(Matrix4& m, const Quaternion& rotation)
	{
		Matrix4 rotationMatrix = rotation.toRotationMatrix();
		m = m * rotationMatrix;
	}
	void scale_matrix(Matrix4& m, const Vector3& scale)
	{
		Matrix4 scaleMatrix(1.0f);
		scaleMatrix[0 + 0 * 4] = scale.x;
		scaleMatrix[1 + 1 * 4] = scale.y;
		scaleMatrix[2 + 2 * 4] = scale.z;
		m = m * scaleMatrix;
	}
	void create_transformation_matrix(Matrix4& outMatrix, const Vector3& position, const Quaternion& rotation, const mml::Vector3& scale)
	{
		Matrix4 transformationMatrix(1.0f);
		translate_matrix(transformationMatrix, position);
		rotate_matrix(transformationMatrix, rotation);
		scale_matrix(transformationMatrix, scale);
		outMatrix = transformationMatrix;
	}
	void create_view_matrix(Matrix4& outMatrix, const Vector3& viewPos, const Quaternion& viewRotation)
	{
		Matrix4 viewMatrix(1.0f);
		rotate_matrix(viewMatrix, viewRotation);
		translate_matrix(viewMatrix, { -viewPos.x,-viewPos.y,-viewPos.z });
		outMatrix = viewMatrix;
	}
}