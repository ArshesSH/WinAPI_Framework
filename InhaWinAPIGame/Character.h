#pragma once

#include "Actor.h"
#include "ActorTag.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "Animation.h"
#include <unordered_map>
#include <string>

class Character: public Actor
{
public:
	Character(ActorTag tag, const Vec2<float>& pos)
		:
		Actor(tag),
		pos(pos)
	{}
	virtual ~Character() {}
protected:

protected:
	//ConvexCollider<float> collider;
	std::unordered_map<int, Animation<int>> animationMap;
	Vec2<float> pos;
	float moveSpeed;
};