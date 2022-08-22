#pragma once

#include "Actor.h"

class Wall : public Actor
{
public:
	Wall( const Vec2<float>& startPos, float halfWidth, float halfHeight )
		:
		Actor( ActorTag::Wall, startPos, std::make_unique<ConvexCollider<float>>( RectF::FromCenter( startPos, halfWidth, halfHeight ) ) )
	{}

	void Update( float dt, class Scene& scene ) override;
	void Draw( HDC hdc ) override
	{
		Gdiplus::Graphics gfx( hdc );
	}
	void SetTransform( const Mat3<float>& transform ) override
	{

	}
	void ApplyDamage( int damage ) override {}

private:

};