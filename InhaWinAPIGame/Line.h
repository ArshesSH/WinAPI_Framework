#pragma once

#include "Vec2.h"

template <typename T>
class Line
{
public:
	Line(){}
	Line(const Vec2<T>& startPos, const Vec2<T>& endPos)
		:
		startPos(startPos),
		endPos(endPos)
	{}
	Line<T> operator+( const Vec2<T>& rhs ) const
	{
		return Line<T>{ startPos + rhs, endPos + rhs };
	}
	Line<T>& operator+=( const Vec2<T>& rhs )
	{
		return *this = *this + rhs;
	}
	Line<T> operator-( const Vec2<T>& rhs ) const
	{
		return Line<T>{ startPos - rhs, endPos - rhs };
	}
	Line<T>& operator-=( const Vec2<T>& rhs )
	{
		return *this = *this - rhs;
	}
	Line<T> operator*( T rhs ) const
	{
		return Line<T>{ startPos * rhs, endPos * rhs };
	}
	Line<T>& operator*=( T rhs )
	{
		return *this = *this * rhs;
	}
	Line<T> operator/( T rhs ) const
	{
		return Line<T>{ startPos/ rhs, endPos/ rhs };
	}
	Line<T>& operator/=( T rhs )
	{
		return *this = *this / rhs;
	}
	bool operator==( const Line<T>& rhs ) const
	{
		return startPos == rhs.startPos && endPos == rhs.endPos;
	}
	bool operator!=( const Line<T>& rhs ) const
	{
		return startPos != rhs.startPos || endPos != rhs.endPos;
	}
	

	Vec2<T> GetStartPos() const
	{
		return startPos;
	}
	Vec2<T> GetEndPos() const
	{
		return endPos;
	}
	Vec2<T> GetLine() const
	{
		return endPos - startPos;
	}
	T GetLengthSq() const
	{
		return GetLine().GetLengthSq();
	}
	T GetLength() const
	{
		return GetLine().GetLength();
	}

	bool IsOverlapWith( const Line<T>& rhs ) const
	{
		float q = (startPos.y - rhs.startPos.y) * (rhs.endPos.x - rhs.startPos.x) - (startPos.x - rhs.startPos.x) * (rhs.endPos.y - rhs.startPos.y);
		float d = (endPos.x - startPos.x) * (rhs.endPos.y - rhs.startPos.y) - (endPos.y - startPos.y) * (rhs.endPos.x - rhs.startPos.x);

		if ( d == 0 )
		{
			return false;
		}
		float r = q / d;

		q = (startPos.y - rhs.startPos.y) * (endPos.x - startPos.x) - (startPos.x - rhs.startPos.x) * (endPos.y - startPos.y);

		float s = q / d;

		if ( r < 0 || r > 1 || s < 0 || s > 1 )
		{
			return false;
		}
		return true;
	}

private:
	Vec2<T> startPos;
	Vec2<T> endPos;
};