#pragma once

#include "Vector3.h"
#include "Quaternion.h"

namespace mml
{
	class Matrix4
	{
	private:
		float _m[16] = { 0 };

	public:

		Matrix4();
		Matrix4(float d);
		// *Note: You can give the array in nice looking ordering(for example in the inputted array: 1,0 means first row, second column)
		// We transpose this ordering in this constructor.
		// ...also we trust here that user understands that u can give only arrays with length of 16
		Matrix4(float* arr);
		Matrix4(const Matrix4& mat);

		void setDiag(float val);
		void setIdentity();
		void transpose();

		
		//	*Found from: https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
		//		comment on the site about this :
		//			"This was lifted from MESA implementation of the GLU library."
		void inverse();

		// Converts PURE rotation matrix into quaternion
		// *Found from: https://math.stackexchange.com/questions/893984/conversion-of-rotation-matrix-to-quaternion
		//		*answerer: SRH
		Quaternion toQuaternion();

		void print() const;

		float& operator[](int i);
		const float& operator[](int i) const; // I have no idea is this kind of duplication legal??

		friend Matrix4 operator*(const Matrix4& left, const Matrix4& right);
		friend Matrix4 operator*(const Matrix4& left, float val);
	};

	void create_orthographic_projection_matrix(
		Matrix4& outMatrix,
		float left, float right,
		float top, float bottom,
		float zNear, float zFar
	);
	void create_perspective_projection_matrix(
		Matrix4& outMatrix, 
		float fov, 
		float aspectRatio, 
		float zNear, 
		float zFar
	);
	
	void translate_matrix(Matrix4& outMatrix, const Vector3& position);
	void rotate_matrix(Matrix4& outMatrix, const Quaternion& rotation);
	void scale_matrix(Matrix4& outMatrix, const Vector3& scale);
	
	void create_transformation_matrix(
		Matrix4& outMatrix, 
		const Vector3& position, 
		const Quaternion& rotation, 
		const mml::Vector3& scale
	);
	
	void create_view_matrix(Matrix4& outMatrix, const Vector3& viewPos, const Quaternion& viewRotation);
}