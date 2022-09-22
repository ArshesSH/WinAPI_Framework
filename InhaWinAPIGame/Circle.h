#pragma once

#include "Vec2.h"
#include "Rect.h"

template <typename T>
class Circle
{
public:
	Circle()
		:
		center( { (T)0, (T)0 } ),
		radius( T( 1 ) )
	{}
	Circle(const Vec2<T>& center, T radius)
		:
		center(center),
		radius(radius)
	{}
	Circle(const _Rect<T>& rect)
		:
		Circle<T>( { rect.left, rect.top }, {rect.right, rect.bottom} )
	{}

	static Circle<T> CreateOuterCircle( const Vec2<T>& topLeft, const Vec2<T>& bottomRight ) 
	{
		Vec2<T> center = (topLeft + bottomRight) / (T)2;
		const Vec2<T> size = bottomRight - topLeft;
		T radius = (std::max)(size.x, size.y);
		return { center, radius };
	}
	static Circle<T> CreateOuterCircle( const _Rect<T>& rect ) 
	{
		return CreateOuterCircle( rect.GetTopLeft(), Vec2<T>{rect.right, rect.bottom} );
	}
	static Circle<T> CreateInnerCircle( const Vec2<T>& topLeft, const Vec2<T>& bottomRight ) 
	{
		Vec2<T> center = (topLeft + bottomRight) / (T)2;
		const Vec2<T> size = bottomRight - topLeft;
		T radius = (std::min)(size.x, size.y);
		return { center, radius };
	}
	static Circle<T> CreateInnerCircle( const _Rect<T>& rect ) 
	{
		return CreateInnerCircle( rect.GetTopLeft(), Vec2<T>{rect.right, rect.bottom} );
	}

	T GetRadius() const
	{
		return radius;
	}
	Vec2<T> GetCenter() const
	{
		return center;
	}
	Vec2<T> GetTopLeft() const
	{
		return { center.x - radius, center.y - radius };
	}
	_Rect<T> GetOuterRect() const
	{
		return { GetTopLeft(), Vec2<T>{center.x + radius, center.y + radius} };
	}
	bool IsOverlapWith( const Circle<T>& other ) const 
	{
		const Vec2<float> distance = GetCenter() - other.GetCenter();
		const float sumOfRadius = GetRadius() + other.GetRadius();
		return fabs( distance.x * distance.x + distance.y * distance.y ) < sumOfRadius * sumOfRadius;
	}

	void SetCenter( const Vec2<T>& pos)
	{
		center = pos;
	}

private:
	Vec2<T> center;
	T radius;
};