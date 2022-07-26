#pragma once

#include "CoordinateTransformer.h"
#include "Rect.h"
#include "Vec2.h"
#include "Mat3.h"
#include "MathSH.h"

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

	Mat3<float> GetTransform() const
	{
		return ct.GetTransform();
	}
	
	template <typename F>
	void Draw( HDC hdc, const Vec2<float>& originPos, F drawFunc )
	{
		transform = (Mat3<float>::Scale( scale ) * Mat3<float>::Translation( pos * ct.GetFactorY() ) * Mat3<float>::Rotation( angle ) );
		ct.Draw( hdc, transform, originPos, std::move(drawFunc) );
	}
	RectF GetScreenRect(int screenWidth, int screenHeight) const
	{
		const float zoom = 1.0f / scale;

		const float digonal = sqrt(
			MathSH::sq( float( screenWidth / 2 ) * zoom ) +
			MathSH::sq( float( screenHeight / 2 ) * zoom )
		);
		return RectF::FromCenter( pos, digonal, digonal );
	}

private:
	CoordinateTransformer& ct;
	Vec2<float> pos = { 0.0f, 0.0f };
	float scale = 1.0f;
	float angle = 0.0f;
	Mat3<float> transform = Mat3<float>::Identity();
};