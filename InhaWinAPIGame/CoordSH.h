#pragma once

#include <ObjIdl.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")

#include <vector>
#include "Vec2.h"

namespace CoordSH
{
	template <typename T>
	struct PointConversion
	{
		Vec2<T> operator()( const Gdiplus::Point& src ) const
		{
			return { (T)src.X, (T)src.Y };
		}

		Gdiplus::Point operator()( const Vec2<T>& src ) const
		{
			return { (int)src.x, (int)src.y };
		}
		Gdiplus::PointF operator()( const Vec2<T>& src ) const
		{
			return { (float)src.x, (float)src.y };
		}

		std::vector<Gdiplus::Point> operator()( const std::vector<Vec2<T>>& src ) const
		{
			std::vector<Gdiplus::Point> points;
			PointConversion conversion;
			points.resize( src.size() );
			for ( int i = 0; i < (int)src.size(); ++i )
			{
				points[i] = conversion( src[i] );
			}
			return points;
		}

		std::vector<Gdiplus::Point> operator()( const std::vector<Vec2<T>>& src ) const
		{
			std::vector<Gdiplus::PointF> points;
			PointConversion conversion;
			points.resize( src.size() );
			for ( int i = 0; i < (int)src.size(); ++i )
			{
				points[i] = conversion( src[i] );
			}
			return points;
		}
	};
}