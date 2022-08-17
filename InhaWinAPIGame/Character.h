#pragma once

#include "Actor.h"
#include "ActorTag.h"
#include "Animation.h"
#include <unordered_map>
#include <string>

class Character : public Actor
{
public:
	Character( ActorTag tag, RectF colliderRect, float moveSpeed, const std::wstring& fileName, const Vec2<float>& imgHalfSize )
		:
		colliderHalfWidth( colliderRect.GetWidth() / 2.0f ),
		colliderHalfHeight( colliderRect.GetHeight() / 2.0f ),
		moveSpeed( moveSpeed ),
		sprite( fileName ),
		imgHalfSize( imgHalfSize ),
		Actor( tag, colliderRect.GetCenter(), std::make_unique<ConvexCollider<float>>( colliderRect ) )
	{}
	virtual ~Character() {}
protected:
	bool IsCollideWithWall( const Vec2<float>& nextPos, const class Scene& scene ) const;
	void Move( float dt, const class Scene& scene );
protected:
	const Vec2<float> dirLeft = { -1.0f, 0.0f };
	const Vec2<float> dirUp = { 0.0f, 1.0f };
	const Vec2<float> dirRight = { 1.0f, 0.0f };
	const Vec2<float> dirDown = { 0.0f, -1.0f };

	std::unordered_map<int, Animation<int>> animationMap;
	Image::ImageGDI<int> sprite;
	Vec2<float> imgHalfSize;

	Vec2<float> dir = { 0.0f, 0.0f };
	float moveSpeed = 0.0f;

	float colliderHalfWidth;
	float colliderHalfHeight;
};