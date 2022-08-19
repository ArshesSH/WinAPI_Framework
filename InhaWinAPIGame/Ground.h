#pragma once

#include "Actor.h"
#include "Line.h"

class Ground : public Actor
{
public:
	Ground( const Line<float>& line )
		:
		Actor( ActorTag::Ground, line.GetStartPos(), std::make_unique<LineCollider<float>>( line ) )
	{}

	void Update( float dt, class Scene& scene );
	void SetTransform( const Mat3<float>& transform )
	{

	}
	void Draw( HDC hdc )
	{

	}

private:

};