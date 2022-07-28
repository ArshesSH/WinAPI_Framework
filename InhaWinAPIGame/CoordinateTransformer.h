#pragma once

#include "Surface.h"
#include "Vec2.h"
#include "Mat3.h"
#include <vector>


class CoordinateTransformer
{
public:
	CoordinateTransformer( HDC hdc )
		:
		hdc( hdc )
	{}

	template <typename F>
	void Draw( Surface<float>& surf, const Vec2<float>& originPos, F drawFunc)
	{
		//Vec2<float> originPos = {(float)game.screenRect.right/2.0f, (float)game.screenRect.bottom / 2.0f };
		surf.ApplyTransformation( Mat3<float>::Translation( originPos ) * Mat3<float>::ScaleIndependent( 1.0f, -1.0f ) );
		drawFunc( hdc );
	}
private:
	HDC hdc;
};