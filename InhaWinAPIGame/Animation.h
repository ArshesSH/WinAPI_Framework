#pragma once

#include <vector>
#include "Surface.h"

class Animation
{
public:
	class Frame
	{
	public:
		Frame(){}
		Frame( const RectI& sprite, float ndcX, float ndcY )
			:
			sprite( sprite ),
			pivot( int( sprite.GetWidth() * ndcX ), int( sprite.GetHeight() * ndcY ) )
		{
		}
		Frame( const RectI& sprite, const Vec2<int>& pivot )
			:
			sprite( sprite ),
			pivot( pivot )
		{}

	public:
		RectI sprite;
		Vec2<int> pivot;
	};

public:

private:
	
	std::vector<Frame> frames;
	float playSpeed;
};