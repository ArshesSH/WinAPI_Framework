#pragma once

#include "Surface.h"
#include "Vec2.h"
#include "Mat3.h"
#include <vector>


class CoordinateTransformer
{
public:

	template <typename F>
	void Draw( HDC hdc, const Mat3<float>& transform, const Vec2<float>& originPos, F drawFunc)
	{
		//Vec2<float> originPos = {(float)game.screenRect.right/2.0f, (float)game.screenRect.bottom / 2.0f };
		//surf.ApplyTransformation( Mat3<float>::Translation( originPos ) * Mat3<float>::ScaleIndependent( 1.0f, -1.0f ) );

		auto coordTrans =  (Mat3<float>::Translation( originPos ) * Mat3<float>::ScaleIndependent( 1.0f, -1.0f )) * transform;

		drawFunc( hdc, coordTrans );
	}
private:
};