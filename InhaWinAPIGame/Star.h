#pragma once

#include "GeometricObject.h"

template<typename T>
class Rect;

template<typename T>
class Star : public GeometricObject<T>
{
public:
	Star( const Vec2<T> center, T outerRadius, int nFlares )
		:
		GeometricObject<T>( center.x, center.y ),
		nFlares( nFlares ),
		outerRadius( outerRadius ),
		dTheta( MathSH::PI / nFlares ),
		innerRadius( (T)((outerRadius* cos( dTheta * 2 )) / (cos( dTheta ))) )
	{
		GeometricObject<T>::vertices.resize( nFlares * 2 );
		SetVertices();
	}
	Star( const Vec2<T> center, T outerRadius, T innerRadius, int nFlares )
		:
		GeometricObject<T>( center.x, center.y ),
		nFlares( nFlares ),
		outerRadius( outerRadius ),
		innerRadius( innerRadius ),
		dTheta( MathSH::PI / nFlares )
	{
		GeometricObject<T>::vertices.resize( nFlares * 2 );
		SetVertices();
	}

	void GetRaidus() const
	{
		return outerRadius;
	}
	T GetWidth() const override
	{
		return outerRadius;
	}
	T GetHeight() const override
	{
		return outerRadius;
	}
	Vec2<T> GetLeftTop() const override
	{
		return { this->center.x - outerRadius, this->center.y - outerRadius };
	}
	Vec2<T> GetRightBottom() const override
	{
		return { this->center.x + outerRadius, this->center.y + outerRadius };
	}
	void SetCenter( const Vec2<T>& center_in ) override
	{
		GeometricObject<T>::center = center_in;
		SetVertices();
	}
	void SetCenter( T x, T y ) override
	{
		SetCenter( { x, y } );
		SetVertices();
	}
	void AddSize( T size_in ) override
	{
		outerRadius += size_in;
		innerRadius = ((T)((outerRadius * cos( dTheta * 2 )) / (cos( dTheta ))));
		SetVertices();
	}
	int GetFlareCount() const override
	{
		return nFlares;
	}

	void Draw( HDC hdc ) const override
	{
		const int size = (int)GeometricObject<T>::vertices.size();
		std::vector<POINT> points;
		points.reserve( size );

		for ( auto e : GeometricObject<T>::vertices )
		{
			const POINT p = { (int)e.x, (int)e.y };
			points.push_back( p );
		}
		Polygon( hdc, &points[0], size );
	}

	void DrawDebug( HDC hdc ) const override
	{
		const int size = (int)GeometricObject<T>::vertices.size();
		std::vector<POINT> points;
		points.reserve( size );

		for ( auto e : GeometricObject<T>::vertices )
		{
			const POINT p = { (int)e.x, (int)e.y };
			points.push_back( p );
		}

		HBRUSH hBrush;
		HBRUSH oldBrush;
		hBrush = CreateSolidBrush( 0x0000FF );
		oldBrush = (HBRUSH)SelectObject( hdc, hBrush );
		Polygon( hdc, &points[0], size );
		SelectObject( hdc, oldBrush );
		DeleteObject( hBrush );
	}

	RECT GetRECT() const override
	{
		const int left = (int)(GeometricObject<T>::center.x - outerRadius);
		const int top = (int)(GeometricObject<T>::center.y - outerRadius);
		const int right = (int)(GeometricObject<T>::center.x + outerRadius);
		const int bottom = (int)(GeometricObject<T>::center.y + outerRadius);
		return { left, top, right, bottom };
	}

	T GetSize() const override
	{
		return outerRadius;
	}
	T GetRadius() const override
	{
		return outerRadius;
	}
private:
	void SetVertices()
	{
		for ( int i = 0; i < nFlares * 2; ++i )
		{
			const double rad = (i % 2 == 0) ? outerRadius : innerRadius;
			const Vec2<T> tranformedVec = GeometricObject<T>::transform * Vec2<T>( (T)(rad * cos( i * dTheta )), (T)(rad * sin( i * dTheta )) );
			GeometricObject<T>::vertices[i] = tranformedVec + GeometricObject<T>::center;
		}
	}

private:
	const int nFlares;
	const double dTheta;
	T outerRadius;
	T innerRadius;
};