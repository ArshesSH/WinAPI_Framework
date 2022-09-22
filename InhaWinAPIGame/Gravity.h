#pragma once

#include "Vec2.h"

class Gravity
{
public:
	Gravity(float factor)
		:
		factor(factor)
	{}
	float GetGravityVel( const Vec2<float>& vel, float dt)
	{
		return vel.y - factor * 100.0f * dt;
	}
	float GetGravity() const
	{
		return factor;
	}
	void SetGravity( float gravity )
	{
		factor = gravity;
	}
private:
	float factor = 0.0f;
};