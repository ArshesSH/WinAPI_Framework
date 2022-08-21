#pragma once

#include "Bullet.h"

class PlayerXBullet : public Bullet
{
public:
	PlayerXBullet( const Vec2<float>& pivotPos, const Vec2<float>& colliderRelativePos )
		:
		Bullet(ActorTag::PlayerBullet, ActorTag::Enemy, pivotPos, colliderRelativePos, )
	{}

private:

};