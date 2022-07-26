#pragma once

#include "GeometricObject.h"

template<typename T>
class Rect;

template<typename T>
class Circle : public GeometricObject<T>
{
public:
	Circle()
	{
		GeometricObject<T>::vertices.push_back( GeometricObject<T>::center );
	}
	Circle( const Vec2<T>& center, T radius )
		:
		GeometricObject<T>( center ),
		radius( radius )
	{
		GeometricObject<T>::vertices.push_back( GeometricObject<T>::center );
	}
	Circle( T x, T y, T radius )
		:
		GeometricObject<T>( x, y ),
		radius( radius )
	{
		GeometricObject<T>::vertices.push_back( GeometricObject<T>::center );
	}
	~Circle() {}

	void SetCenter( const Vec2<T>& p ) override
	{
		GeometricObject<T>::center = p;
		GeometricObject<T>::vertices[0] = GeometricObject<T>::center;
	}
	void SetCenter( T x, T y ) override
	{
		GeometricObject<T>::center = { x, y };
		GeometricObject<T>::vertices[0] = GeometricObject<T>::center;
	}
	void SetRadius( T r )
	{
		radius = r;
	}
	Vec2<T> GetLeftTop() const override
	{
		return { this->center.x - radius, this->center.y - radius };
	}
	Vec2<T> GetRightBottom() const override
	{
		return { this->center.x + radius, this->center.y + radius };
	}
	T GetRadius() const override
	{
		return radius;
	}
	T GetWidth() const override
	{
		return radius;
	}
	T GetHeight() const override
	{
		return radius;
	}
	T GetSize() const override
	{
		return radius;
	}
	void AddSize( T size_in ) override
	{
		radius += size_in;
	}
	RECT GetRECT() const override
	{
		const int left = (int)(GeometricObject<T>::center.x - radius);
		const int top = (int)(GeometricObject<T>::center.y - radius);
		const int right = (int)(GeometricObject<T>::center.x + radius);
		const int bottom = (int)(GeometricObject<T>::center.y + radius);
		return { left, top, right, bottom };
	}
	void Draw( HDC hdc ) const override
	{
		const int left = (int)(GeometricObject<T>::center.x - radius);
		const int top = (int)(GeometricObject<T>::center.y - radius);
		const int right = (int)(GeometricObject<T>::center.x + radius);
		const int bottom = (int)(GeometricObject<T>::center.y + radius);

		Ellipse( hdc, left, top, right, bottom );
	}
	void DrawDebug( HDC hdc ) const override
	{
		DrawColor( hdc, RGB( 255, 0, 0 ) );
	}
	void DrawColor( HDC hdc, COLORREF color = 0xFFFFFF ) const
	{
		const int left = (int)(GeometricObject<T>::center.x - radius);
		const int top = (int)(GeometricObject<T>::center.y - radius);
		const int right = (int)(GeometricObject<T>::center.x + radius);
		const int bottom = (int)(GeometricObject<T>::center.y + radius);

		HBRUSH hBrush;
		HBRUSH oldBrush;
		hBrush = CreateSolidBrush( color );
		oldBrush = (HBRUSH)SelectObject( hdc, hBrush );
		Ellipse( hdc, left, top, right, bottom );
		SelectObject( hdc, oldBrush );
		DeleteObject( hBrush );
	}
private:
	T radius;
};