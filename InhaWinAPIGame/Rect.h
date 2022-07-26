#pragma once

#include "GeometricObject.h"


template<typename T>
class Rect : public GeometricObject<T>
{
public:
	Rect()
		:
		GeometricObject<T>( (T)0, (T)0 ),
		width( (T)1 ),
		height( (T)1 ),
		halfWidth( (T)(width / 2) ),
		halfHeight( (T)(height / 2) )
	{
		GeometricObject<T>::vertices.resize( 4 );
		SetVertices();
	}
	Rect( const Vec2<T>& center, T width, T height )
		:
		GeometricObject<T>( center ),
		width( width ),
		height( height ),
		halfWidth( (T)(width / 2) ),
		halfHeight( (T)(height / 2) )
	{
		GeometricObject<T>::vertices.resize( 4 );
		SetVertices();
	}
	Rect( T x, T y, T width, T height )
		:
		GeometricObject<T>( x, y ),
		width( width ),
		height( height ),
		halfWidth( (T)(width / 2) ),
		halfHeight( (T)(height / 2) )
	{
		GeometricObject<T>::vertices.resize( 4 );
		SetVertices();
	}
	Rect( const Vec2<T>& topLeft, const Vec2<T>& bottomRight )
		:
		GeometricObject<T>( (topLeft + bottomRight)* (T)0.5 ),
		width( bottomRight.x - topLeft.x ),
		height( bottomRight.y - topLeft.y ),
		halfWidth( (T)(width / 2) ),
		halfHeight( (T)(height / 2) )
	{
		GeometricObject<T>::vertices.resize( 4 );
		SetVertices();
	}

	~Rect() {}

	T GetRadius() const override
	{
		return (T)std::sqrt( halfWidth * halfWidth + halfWidth * halfWidth );
	}
	T GetWidth() const override
	{
		return width;
	}
	T GetHeight() const override
	{
		return height;
	}
	Vec2<T> GetLeftTop() const override
	{
		return { this->center.x - halfWidth, this->center.y - halfHeight };
	}
	Vec2<T> GetRightBottom() const override
	{
		return { this->center.x + halfWidth, this->center.y + halfHeight };
	}
	void SetWidth( T width_in )
	{
		width = width_in;
		SetVertices();
	}
	void SetHeight( T height_in )
	{
		height = height_in;
		SetVertices();
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
	T GetSize() const override
	{
		return (std::max)(width, height);
	}
	void AddSize( T size_in ) override
	{
		width += size_in;
		height += size_in;
		SetVertices();
	}
	void Draw( HDC hdc ) const override
	{
		std::vector<POINT> points;
		for ( auto e : GeometricObject<T>::vertices )
		{
			points.push_back( { (LONG)e.x, (LONG)e.y } );
		}
		Polygon( hdc, &points[0], (int)points.size() );
	}
	void DrawDebug( HDC hdc ) const override
	{
		std::vector<POINT> points;
		for ( auto e : GeometricObject<T>::vertices )
		{
			points.push_back( { (LONG)e.x, (LONG)e.y } );
		}

		HBRUSH hBrush;
		HBRUSH oldBrush;
		hBrush = CreateSolidBrush( 0x0000FF );
		oldBrush = (HBRUSH)SelectObject( hdc, hBrush );
		Polygon( hdc, &points[0], (int)points.size() );
		SelectObject( hdc, oldBrush );
		DeleteObject( hBrush );
	}
	RECT GetRECT() const override
	{
		return { (int)left, (int)top, (int)right, (int)bottom };
	}

private:
	void SetVertices()
	{
		left = GeometricObject<T>::center.x - halfWidth;
		right = GeometricObject<T>::center.x + halfWidth;
		top = GeometricObject<T>::center.y - halfHeight;
		bottom = GeometricObject<T>::center.y + halfHeight;

		GeometricObject<T>::vertices[0] = GeometricObject<T>::transform * (GeometricObject<T>::center - Vec2<T>{ (T)left, (T)top }) + GeometricObject<T>::center;
		GeometricObject<T>::vertices[1] = GeometricObject<T>::transform * (GeometricObject<T>::center - Vec2<T>{ (T)right, (T)top }) + GeometricObject<T>::center;
		GeometricObject<T>::vertices[2] = GeometricObject<T>::transform * (GeometricObject<T>::center - Vec2<T> { (T)right, (T)bottom }) + GeometricObject<T>::center;
		GeometricObject<T>::vertices[3] = GeometricObject<T>::transform * (GeometricObject<T>::center - Vec2<T>{ (T)left, (T)bottom }) + GeometricObject<T>::center;
	}

private:
	T width;
	T height;
	T left;
	T right;
	T top;
	T bottom;
	T halfWidth;
	T halfHeight;
};