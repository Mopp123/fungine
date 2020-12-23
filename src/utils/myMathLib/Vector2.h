#pragma once

// "mml" stands for: "my math library"
namespace mml
{
	// *This can be used only as a container for integers..
	struct IVector2
	{
		int x, y;
		IVector2(int X, int Y) : x(X), y(Y) {}
	};

	class Vector2
	{
	public:
		float x = 0.0f;
		float y = 0.0f;

		Vector2() {}
		Vector2(float X, float Y);

		float magnitude() const;
		void normalize();
		
		float dot(const Vector2& other) const;

		friend Vector2 operator+(const Vector2& left, const Vector2& right);
		friend Vector2 operator-(const Vector2& left, const Vector2& right);
		friend Vector2 operator*(const Vector2& left, const Vector2& right);
		friend Vector2 operator*(const Vector2& left, float right);
	
	};
}