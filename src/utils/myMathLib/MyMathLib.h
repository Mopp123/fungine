#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

namespace mml
{
	float get_triangle_height_barycentric(
		const mml::Vector3& p1,
		const mml::Vector3& p2,
		const mml::Vector3& p3,
		const mml::Vector2& pos
	);

	float to_radians(float degrees);
}