#pragma once

#include "Actor.h"
#include "ActorTag.h"
#include "Animation.h"
#include <unordered_map>
#include <string>

class Character: public Actor
{
public:
	Character( CollisionManager<float>& cm, ActorTag tag, const Vec2<float>& startPos, float colliderHalfWidth, float colliderHalfHeight )
		:
		colliderHalfWidth( colliderHalfWidth ),
		colliderHalfHeight( colliderHalfHeight ),
		Actor( cm, tag, startPos, std::make_unique<ConvexCollider<float>>( RectF::FromCenter( startPos, colliderHalfWidth, colliderHalfWidth ) ) )
	{}
	virtual ~Character() {}
protected:
	bool IsCollideWithWall( const Vec2<float>& nextPos, const class Scene& scene ) const;
	void Move( float dt, const class Scene& scene );

protected:
	std::unordered_map<int, Animation<int>> animationMap;

	Vec2<float> dir;
	float moveSpeed = 0.0f;

	float colliderHalfWidth;
	float colliderHalfHeight;
};