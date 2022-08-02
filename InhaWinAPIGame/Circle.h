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
		Circle( { rect.left, rect.top }, {rect.right, rect.bottom} )
	{}

	static Circle CreateOuterCircle( const Vec2<T>& topLeft, const Vec2<T>& bottomRight ) const
	{
		Vec2<T> center = (topLeft + bottomRight) / (T)2;
		const Vec2<T> size = bottomRight - topLeft;
		T radius = (std::max)(size.x, size.y);
		return { center, radius };
	}
	static Circle CreateOuterCircle( const _Rect<T>& rect ) const
	{
		return CreateOuterCircle( rect.GetTopLeft(), Vec2<T>{rect.right, rect.bottom} );
	}
	static Circle CreateInnerCircle( const Vec2<T>& topLeft, const Vec2<T>& bottomRight ) const
	{
		Vec2<T> center = (topLeft + bottomRight) / (T)2;
		const Vec2<T> size = bottomRight - topLeft;
		T radius = (std::min)(size.x, size.y);
		return { center, radius };
	}
	static Circle CreateInnerCircle( const _Rect<T>& rect ) const
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

private:
	Vec2<T> center;
	T radius;
};