#pragma once

#include "Character.h"

class Enemy : public Character
{
public:
	Enemy(const Vec2<float>& pivotPos, const Vec2<float>& colliderRelativePos, float colliderHalfWidth, float colliderHalfHeight,
		float moveSpeed, const std::wstring& fileName, const std::wstring& flipPath )
		:
		Character(ActorTag::Enemy, pivotPos, RectF::FromCenter( colliderRelativePos, colliderHalfWidth, colliderHalfHeight ), moveSpeed, fileName, flipPath )
	{}
private:

};