#pragma once

#include "Vec2.h"
#include "Mat3.h"
#include "MathSH.h"
#include "framework.h"
#include <vector>

template<typename T>
class GeometricObject
{
public:
	GeometricObject()
		:
		center( (T)0, (T)0 )
	{
	}
	GeometricObject( const Vec2<T> center )
		:
		center( center )
	{
	}
	GeometricObject( T x, T y )
		:
		center( x, y )
	{
	}
	virtual ~GeometricObject() {}

	virtual T GetRadius() const = 0;
	virtual RECT GetRECT() const { return { 0,0,0,0 }; }
	virtual T GetSize() const = 0;
	virtual T GetWidth() const = 0;
	virtual T GetHeight() const = 0;
	virtual void AddSize( T size_in ) = 0;
	virtual void Draw( HDC hdc ) const = 0;
	virtual void DrawDebug( HDC hdc ) const = 0;
	virtual int GetFlareCount() const { return 0; };
	double GetDistanceWith( const GeometricObject<T>& other ) const
	{
		const Vec2<T> vDist = other.center - center;
		return vDist.GetLength();
	}
	Vec2<T> GetCenter() const
	{
		return center;
	}
	T GetCenterX() const
	{
		return center.x;
	}
	T GetCenterY() const
	{
		return center.y;
	}
	virtual Vec2<T> GetLeftTop() const = 0;
	virtual Vec2<T> GetRightBottom() const = 0;

	std::vector<Vec2<T>> GetVertices() const
	{
		return vertices;
	}
	POINT Vec2ToPoint( const Vec2<T>& v ) const
	{
		return POINT( (int)v.x, (int)v.y );
	}
	Vec2<T> PointToVec2( const POINT& p ) const
	{
		return Vec2<T>( (T)p.x, (T)p.y );
	}

	virtual void SetCenter( const Vec2<T>& p )
	{
		center = p;
	}
	virtual void SetCenter( T x, T y )
	{
		center = { x, y };
	}
	void SetCenterX( T x )
	{
		center.x = x;
	}
	void SetCenterY( T y )
	{
		center.y = y;
	}
	void ApplyTransformation( const Mat3<T>& transform_in )
	{
		transform = transform_in;
	}

protected:
	Vec2<T> center;
	std::vector<Vec2<T>> vertices;
	Mat3<T> transform = Mat3<T>::Identity();
};


