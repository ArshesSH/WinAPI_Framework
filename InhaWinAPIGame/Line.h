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
		float q = (startPos.Y - rhs.startPos.Y) * (rhs.endPos.X - rhs.startPos.X) - (startPos.X - rhs.startPos.X) * (rhs.endPos.Y - rhs.startPos.Y);
		float d = (endPos.X - startPos.X) * (rhs.endPos.Y - rhs.startPos.Y) - (endPos.Y - startPos.Y) * (rhs.endPos.X - rhs.startPos.X);

		if ( d == 0 )
		{
			return false;
		}
		float r = q / d;

		q = (startPos.Y - rhs.startPos.Y) * (endPos.X - startPos.X) - (startPos.X - rhs.startPos.X) * (endPos.Y - startPos.Y);

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