#pragma once

#include "Actor.h"
#include "ActorTag.h"
#include "Collider.h"

class Wall : public Actor
{
public:
	Wall( const Vec2<float>& topLeft, float width, float height )
		:
		Actor(ActorTag::Wall),
		collider(topLeft, width, height)
	{}
	void Update( float dt, class Game& game ) override;
	void Draw( HDC hdc ) override
	{
		//Debug
		Gdiplus::Graphics gfx(hdc);
		collider.Draw( gfx, { 255,255,255,255 } );
	}

private:
	ConvexCollider<float> collider;
};