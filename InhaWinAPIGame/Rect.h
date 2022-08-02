#pragma once

#include "Vec2.h"
#include <algorithm>

template < typename T >
class _Rect
{
public:
	inline	_Rect() {}
	inline	_Rect( T top, T bottom, T left, T right )
		:
		top( top ),
		bottom( bottom ),
		left( left ),
		right( right )
	{}
	inline	_Rect( const _Rect& rect )
		:
		top( rect.top ),
		bottom( rect.bottom ),
		left( rect.left ),
		right( rect.right )
	{}
	inline	_Rect( Vec2<T> p0, Vec2<T> p1 )
		:
		_Rect( min( p0.y, p1.y ),
			max( p0.y, p1.y ),
			min( p0.x, p1.x ),
			max( p0.x, p1.x ) )
	{}
	inline	void Translate( Vec2<T> d )
	{
		Translate( d.x, d.y );
	}
	inline	void Translate( T dx, T dy )
	{
		top += dy;
		bottom += dy;
		left += dx;
		right += dx;
	}
	template <typename T2>
	inline	operator _Rect<T2>() const
	{
		return { (T2)top,(T2)bottom,(T2)left,(T2)right };
	}
	template <typename T2>
	inline _Rect<T> operator+( const Vec2<T2>& rhs ) const
	{
		return { top + (T)rhs.y, bottom + (T)rhs.y, left + (T)rhs.x, right + (T)rhs.x };
	}
	template <typename T2>
	inline _Rect<T>& operator+=( const Vec2<T2>& rhs ) 
	{
		return *this = *this + rhs;
	}
	template <typename T2>
	inline _Rect<T> operator-( const Vec2<T2>& rhs ) const
	{
		return { top - (T)rhs.y, bottom - (T)rhs.y, left - (T)rhs.x, right - (T)rhs.x };
	}
	template <typename T2>
	inline _Rect<T>& operator-=( const Vec2<T2>& rhs )
	{
		return *this = *this - rhs;
	}


	inline	void ClipTo( const _Rect& rect )
	{
		top = (std::max)( top, rect.top );
		bottom = (std::min)( bottom, rect.bottom );
		left = (std::max)( left, rect.left );
		right = (std::min)( right, rect.right );
	}
	inline	T GetWidth() const
	{
		return right - left;
	}
	inline	T GetHeight() const
	{
		return bottom - top;
	}
	inline	bool Overlaps( const _Rect& rect ) const
	{
		return top < rect.bottom&& bottom > rect.top &&
			left < rect.right&& right > rect.left;
	}
	template <typename T2>
	inline	bool Contains( Vec2<T2> p ) const
	{
		return p.y >= top && p.y <= bottom && p.x >= left && p.x <= right;
	}
	template <typename T2>
	inline	bool Contains( _Rect<T2> p ) const
	{
		return p.top >= top && p.bottom <= bottom && p.left >= left && p.right <= right;
	}

	static _Rect<T> FromCenter( const Vec2<T>& center, float halfWidth, float halfHeight )
	{
		const Vec2<T> half( halfWidth, halfHeight );
		return _Rect<T>( center - half, center + half );
	}
	inline Vec2<T> GetTopLeft() const
	{
		return { left, top };
	}
	inline Vec2<T> GetCenter() const
	{
		const auto topLeft = GetTopLeft();
		const auto halfWidth = GetWidth() / (T)2;
		const auto halfHeight = GetHeight() / (T)2;

		return { topLeft.x + halfWidth, topLeft.y + halfHeight };
	}

public:
	T top;
	T bottom;
	T left;
	T right;
};

typedef _Rect< int > RectI;
typedef _Rect< float > RectF;