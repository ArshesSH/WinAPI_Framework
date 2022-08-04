#pragma once

#include "Actor.h"
#include "Surface.h"
#include "Image.h"
#include "Collider.h"

#include <string>

template <typename T>
class Character: public Actor
{
public:
	Character(const std::wstring& imageName)
		:
		image(imageName)
	{}

protected:
	virtual void Control( float dt ) = 0;
	virtual bool CanMove( const Vec2<T>& pos ) = 0;
	virtual void Move( float dt, const Vec2<T>& dir )
	{
		nextPos = pos + dir;
		if ( CanMove( nextPos ) )
		{
			pos = nextPos;
		}
	}

protected:
	Image::ImageGDIPlus<T> image;
	Surface<T> surf;
	ConvexCollider<T> collider;
	Vec2<T> pos;
	Vec2<T> nextPos;
	T speed;
};