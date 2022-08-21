#pragma once

#include "Character.h"

class Bullet : public Character
{
public:
	Bullet(ActorTag tag, ActorTag oppositeTag, const Vec2<float>& pivotPos, const Vec2<float>& colliderRelativePos, const Vec2<float>& colliderHalfSize,
		float moveSpeed, int damage, const std::wstring& fileName, const std::wstring& flipPath)
		:
		oppositeTag(oppositeTag),
		damage(damage),
		Character(tag, pivotPos, RectF::FromCenter(colliderRelativePos, colliderHalfSize.x, colliderHalfSize.y), moveSpeed, fileName, flipPath )
	{}

	void Update( float dt, class Scene& scene ) override;

private:
	int damage = 0;
	ActorTag oppositeTag;
};