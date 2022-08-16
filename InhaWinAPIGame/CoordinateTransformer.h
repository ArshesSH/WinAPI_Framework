#pragma once

#include "Surface.h"
#include "Vec2.h"
#include "Mat3.h"
#include <vector>

class CoordinateTransformer
{
public:
	CoordinateTransformer() {}
	CoordinateTransformer( const Mat3<float>& scaler )
		:
		scale( scaler ),
		yFactor( 1 )
	{}

	template <typename F>
	void Draw( HDC hdc, const Mat3<float>& transform, const Vec2<float>& originPos, F drawFunc)
	{
		auto coordTrans =  (Mat3<float>::Translation( originPos ) * scale) * transform;
		drawFunc( hdc, coordTrans );
	}

	float GetFactorY() const
	{
		return yFactor;
	}

private:
	Mat3<float> scale = Mat3<float>::ScaleIndependent( 1.0f, -1.0f );
	float yFactor = -1;
};