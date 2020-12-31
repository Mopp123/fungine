#pragma once

#include "Vector3.h"

// "mml" stands for: "my math library"
namespace mml
{
	// *This can be used only as a container for integers..
	struct IVector4 : public IVector3
	{
		int w = 0;
		IVector4() {}
		IVector4(int X, int Y, int Z, int W) : IVector3(X, Y, Z), w(W) {}
	};

	class Vector4
	{
	public:
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float w = 0.0f;

		Vector4() {}
		Vector4(float X, float Y, float Z, float W);

		float magnitude() const;
		void normalize();

		friend float dot(const Vector4& left, const Vector4& right);
		
		friend Vector4 operator+(const Vector4& left, const Vector4& right);
		friend Vector4 operator-(const Vector4& left, const Vector4& right);
		friend Vector4 operator*(const Vector4& left, const Vector4& right);
		friend Vector4 operator*(const Vector4& left, float right);

	};
}