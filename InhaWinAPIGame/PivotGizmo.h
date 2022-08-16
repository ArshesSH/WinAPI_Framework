#pragma once

#include "Vec2.h"
#include "Surface.h"

class PivotGizmo
{
public:
	PivotGizmo(){}
	PivotGizmo(const Vec2<int>& pos)
		:
		pos(pos)
	{}
	PivotGizmo(int x, int y)
		:
		PivotGizmo( Vec2<int>{x, y} )
	{}
	
	Vec2<int> GetPos() const
	{
		return pos;
	}
	void SetPos( const Vec2<int>& pos_in )
	{
		pos = pos_in;
	}
	
	void Draw(HDC hdc)
	{
		Vec2<int> startPos = pos - xFactor;
		Vec2<int> endPos = pos + xFactor;
		surf.DrawLineGDI( hdc, startPos, endPos, color );

		startPos = pos - yFactor;
		endPos = pos + yFactor;
		surf.DrawLineGDI( hdc, startPos, endPos, color );
	}

	void SetTransform( const Mat3<float>& transform )
	{
		surf.SetTransformation( transform );
	}
	
private:
	static constexpr int length = 5;
	const Vec2<int> xFactor = { length, 0 };
	const Vec2<int> yFactor = { 0, length };
	Surface<int> surf;
	Vec2<int> pos;
	COLORREF color = RGB( 255, 0, 0 );
};