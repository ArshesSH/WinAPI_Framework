#pragma once

#include "Character.h"

class Bullet : public Character
{
public:
	Bullet(ActorTag tag, ActorTag oppositeTag, const Vec2<float>& pivotPos, const Vec2<float>& colliderRelativePos, const Vec2<float>& colliderHalfSize,
		float moveSpeed, const Vec2<float>& dir, int damage, const std::wstring& fileName, const std::wstring& flipPath)
		:
		oppositeTag(oppositeTag),
		damage(damage),
		Character(tag, pivotPos, RectF::FromCenter(colliderRelativePos, colliderHalfSize.x, colliderHalfSize.y), moveSpeed, fileName, flipPath )
	{
		vel = dir * moveSpeed;
	}

	void Update( float dt, class Scene& scene ) override;
	void ApplyDamage( int damage ) override {}

protected:
	int damage = 0;
	float lifeCycle = 3.0f;
	ActorTag oppositeTag;
	float time = 0.0f;

	
};