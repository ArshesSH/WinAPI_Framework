#pragma once

#include "CoordinateTransformer.h"
#include "Rect.h"
#include "Vec2.h"
#include "Mat3.h"

class Camera
{
public:
	Camera( CoordinateTransformer& ct, const Vec2<float>& pos = {0.0f, 0.0f}, float scale = 1.0f, float angle = 0.0f )
		:
		ct(ct),
		pos(pos),
		scale(scale),
		angle(angle)
	{}

	const Vec2<float>& GetPos() const
	{
		return pos;
	}
	void SetPos( const Vec2<float>& pos_in )
	{
		pos = pos_in;
	}
	float GetAngle() const
	{
		return angle;
	}
	void SetAngle( float angle_in )
	{
		angle = angle_in;
	}
	float GetScale() const
	{
		return scale;
	}
	void SetScale( float scale_in )
	{
		scale = scale_in;
	}
	void RotateBy( float angle_in )
	{
		angle += angle_in;
	}
	void MoveBy( const Vec2<float>& offset )
	{
		pos += offset;
	}
	void ScaledBy( float scale_in )
	{
		scale *= scale_in;
	}

	template <typename F>
	void Draw( const Vec2<float>& originPos, F drawFunc )
	{
		surf.ApplyTransformation( Mat3<float>::Rotation( angle ) * Mat3<float>::Scale( scale ) * Mat3<float>::Translation( -pos ) );
		ct.Draw( std::move(surf), originPos, std::move(drawFunc) );
	}
	//const RectF& GetScreenRect()
	//{
	//	const float zoom = 1.0f / scale;

	//	const float digonal = sqrt(
	//		sq( float( Graphics::ScreenWidth / 2 ) * zoom ) +
	//		sq( float( Graphics::ScreenHeight / 2 ) * zoom )
	//	);
	//	return RectF::FromCenter( pos, digonal, digonal );
	//}*/

private:
	CoordinateTransformer& ct;
	Surface<float> surf;
	Vec2<float> pos = { 0.0f, 0.0f };
	float scale = 1.0f;
	float angle = 0.0f;
};