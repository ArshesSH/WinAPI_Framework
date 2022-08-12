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
		//void SetPivotFromNDC( const Vec2<float>& ndc )
		//{
		//	pivot = sprite.GetTopLeft() + Vec2<int>( int( sprite.GetWidth() * ndc.x ), int( sprite.GetHeight() * ndc.y ) );
		//}
		//static Vec2<int> CalcPivotFromNDC( const Vec2<int>& pos, const Vec2<float>& ndc, int width, int height )
		//{
		//	return pos + Vec2<int>( int( width * ndc.x ), int( height * ndc.y ) );
		//}

	public:
		RectI sprite;
		Vec2<int> pivot;
	};

public:

private:
	std::vector<Frame> frames;
	float playSpeed;
};