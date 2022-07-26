#pragma once

#include <cassert>
#include <vector>
#include <list>
#include "Vec2.h"
#include "Surface.h"
#include "IndexedLineList.h"
#include "Ray.h"

namespace ArshesSH
{
	struct PointConversion
	{
		Vec2<int> operator()( const Gdiplus::Point& src ) const
		{
			return { src.X, src.Y };
		}

		Gdiplus::Point operator()( const Vec2<int>& src ) const
		{
			return { src.x, src.y };
		}

		std::vector<Gdiplus::Point> operator()( const std::vector<Vec2<int>>& src ) const
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
	};


	class Polygon
	{
	public:
		Polygon() {}
		Polygon( const std::vector<Vec2<int>>& vertices )
			:
			vertices( vertices )
		{
		}

		Polygon operator+( const Vec2<int>& pos ) const
		{
			Polygon p = *this;
			for ( auto& v : p.vertices )
			{
				v += pos;
			}
			return p;
		}
		Polygon& operator+=( const Vec2<int>& pos )
		{
			for ( auto& v : vertices )
			{
				v += pos;
			}
			return *this;
		}
		static std::vector<Gdiplus::Point> ConvertToVectorOfPoint( const Polygon& src )
		{
			PointConversion conversion;
			return conversion( src.vertices );
		}

		void push_back( const Vec2<int>& pos )
		{
			vertices.push_back( pos );
		}
		void push_back( int x, int y )
		{
			vertices.emplace_back( x, y );
		}
		size_t size() const
		{
			return vertices.size();
		}

		std::pair<int, int> GetIndex( const Vec2<int>& pos ) const
		{
			for ( int i = 0; i < (int)vertices.size(); ++i )
			{
				if ( IsOnLine( pos, i, GetSafeIndex( i + 1 ) ) )
				{
					return { i, i + 1 };
				}
			}
			return { -1, -1 };
		}
		std::pair<Vec2<int>, Vec2<int>> GetLineVertices( int startIdx, int endIdx ) const
		{
			return { vertices[startIdx], vertices[endIdx] };
		}
		Vec2<int> GetLineVec2( int startIdx, int endIdx ) const
		{
			return vertices[endIdx] - vertices[startIdx];
		}

		int GetSafeIndex( int i ) const
		{
			if ( i >= (int)vertices.size() )
			{
				i %= (int)vertices.size();
			}
			else if ( i < 0 )
			{
				i = GetSafeIndex( (int)vertices.size() + i );
			}
			return i;
		}
		bool IsHorizontal( const Vec2<int>& lineStart, const Vec2<int>& lineEnd ) const
		{
			return (lineStart.y == lineEnd.y);
		}
		bool IsHorizontal( int startIdx, int endIdx ) const
		{
			return vertices[startIdx].y == vertices[endIdx].y;
		}
		bool IsVertical( const Vec2<int>& lineStart, const Vec2<int>& lineEnd ) const
		{
			return (lineStart.x == lineEnd.x);
		}
		bool IsVertical( int startIdx, int endIdx ) const
		{
			return vertices[startIdx].x == vertices[endIdx].x;
		}
		bool IsOnAnyVertex( const Vec2<int>& pos ) const
		{
			for ( const auto& v : vertices )
			{
				if ( v == pos )
				{
					return true;
				}
			}
			return false;
		}
		bool IsOnLine( const Vec2<int>& pos, int startIdx, int endIdx ) const
		{
			if ( IsHorizontal( startIdx, endIdx ) )
			{
				const int minX = (std::min)(vertices[startIdx].x, vertices[endIdx].x);
				const int maxX = (std::max)(vertices[startIdx].x, vertices[endIdx].x);

				return pos.x >= minX &&
					pos.x <= maxX &&
					pos.y == vertices[startIdx].y;
			}
			else if ( IsVertical( startIdx, endIdx ) )
			{
				const int minY = (std::min)(vertices[startIdx].y, vertices[endIdx].y);
				const int maxY = (std::max)(vertices[startIdx].y, vertices[endIdx].y);

				return pos.x >= minY &&
					pos.x <= maxY &&
					pos.y == vertices[startIdx].x;
			}
			return false;
		}
		bool IsOnFirstVertex( const Vec2<int>& pos, int startIdx ) const
		{
			return pos == vertices[startIdx];
		}
		bool IsOnSecondVertex( const Vec2<int>& pos, int endIdx ) const
		{
			return pos == vertices[endIdx];
		}
		bool IsHas( const Vec2<int>& pos, const Vec2<int>& rayStartPos = {0,0} ) const
		{
			Ray<float> ray( (Vec2<float>)pos );
			bool isIn = false;
			for ( int i = 0; i < (int)vertices.size(); ++i )
			{
				const auto pairVertices = GetLineVertices( GetSafeIndex( i ), GetSafeIndex( i + 1 ) );
				if ( ray.CastToLine( (Vec2<float>)pairVertices.first, (Vec2<float>)pairVertices.second ) )
				{
					isIn = !isIn;
				}
			}
			return isIn;
		}
		std::pair<int, int> GetFactoredIndices( const std::pair<int, int>& indices, int factor )
		{
			return { GetSafeIndex( indices.first + factor ), GetSafeIndex( indices.second + factor ) };
		}

	private:


	public:
		
	private:
		std::vector<Vec2<int>> vertices;
	};
}