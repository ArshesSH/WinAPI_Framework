#pragma once

#include "Actor.h"
#include "Collider.h"
#include "Animation.h"
#include <unordered_map>
#include <string>

template <typename T>
class Character: public Actor
{
public:
	Character(const Vec2<T>& pos)
		:
		pos(pos)
	{}

protected:
	virtual void Move( float dt, const Vec2<T>& dir )
	{
		const Vec2<T> nextPos = pos + (dir * moveSpeed * dt);
		if ( CanMove( nextPos ) )
		{
			pos = nextPos;
		}
	}

protected:
	ConvexCollider<T> collider;
	std::unordered_map<int, Animation<T>> animationMap;
	Vec2<T> pos;
	T moveSpeed;
};