#pragma once

#include <vector>
#include "framework.h"
#include "Surface.h"

#include "Vec2.h"
#include "Rect.h"
#include "Circle.h"
#include "Line.h"

template <typename T>
class Collider
{
public:
	enum class Type
	{
		Convex,
		Circle,
		Line
	};

public:
	Collider( Type type, const _Rect<T>& rect )
		:
		type(type),
		rect( rect )
	{}
	void UpdateMatrix( const Mat3<float>& transform )
	{
		surf.SetTransformation( transform );
	}
	_Rect<T> GetRect() const
	{
		return rect;
	}
	Vec2<T> GetPos() const
	{
		return rect.GetTopLeft();
	}
	Vec2<T> GetCenter() const
	{
		return rect.GetCenter();
	}

	virtual std::vector<Vec2<T>> GetVertices() const { return {}; }
	virtual void Draw( Gdiplus::Graphics& gfx, const Gdiplus::Color& color ) = 0;
	virtual Circle<T> GetCircle() const
	{
		return Circle<T>::CreateOuterCircle( rect );
	}
	virtual void SetPos( const Vec2<T>& pos_in )
	{
		const Vec2<T> delta = pos_in - rect.GetTopLeft();
		rect += delta;
	}
	virtual void SetPosByCenter( const Vec2<T>& pos_in )
	{
		const Vec2<T> delta = pos_in - rect.GetCenter();
		rect += delta;
	}
	virtual void SetSize( const Vec2<T>& size )
	{
		rect = rect.ResizeFromCenter( size );
	}
	virtual void MoveBy( const Vec2<T>& offset )
	{
		rect += offset;
	}
	virtual Vec2<T> GetDir() = 0;
	Type GetType() const
	{
		return type;
	}

protected:
	Type type;
	Surface<T> surf;
	_Rect<T> rect;
	Gdiplus::Color debugColor = { 144,255,0,255 };
};

template <typename T>
class ConvexCollider : public Collider<T>
{
public:
	ConvexCollider( const Vec2<T>& pos, const T& width, const T& height )
		:
		ConvexCollider( _Rect<T>{ pos, pos + Vec2<T>{width, height} } )
	{}
	ConvexCollider(const _Rect<T>& rect)
		:
		Collider<T>( Collider<T>::Type::Convex, rect )
	{
		vertices.reserve( 4 );
		vertices.emplace_back( rect.left, rect.top );
		vertices.emplace_back( rect.right, rect.top );
		vertices.emplace_back( rect.right, rect.bottom );
		vertices.emplace_back( rect.left, rect.bottom );

		CalcRect();
	}
	ConvexCollider( const std::vector<Vec2<T>>& vertices, const Vec2<T>& pos )
		:
		Collider<T>( Collider<T>::Type::Convex, pos ),
		vertices( vertices )
	{}

	_Rect<T> CalcRect() const
	{
		Vec2<T> topLeft;
		Vec2<T> bottomRight;
		for ( const auto& v : vertices )
		{
			topLeft.x = (std::min)(topLeft.x, v.x);
			topLeft.y = (std::min)(topLeft.x, v.y);
			bottomRight.x = (std::max)(bottomRight.x, v.x);
			bottomRight.y = (std::max)(bottomRight.y, v.y);
		}
		return { topLeft, bottomRight };
	}

	void SetPos( const Vec2<T>& pos ) override
	{
		const Vec2<T> moved = pos - Collider<T>::rect.GetTopLeft();
		Collider<T>::SetPos( pos );
		for ( auto& v : vertices )
		{
			v += moved;
		}
	}
	void SetPosByCenter( const Vec2<T>& pos ) override
	{
		const Vec2<T> moved = pos - Collider<T>::rect.GetCenter();
		Collider<T>::SetPosByCenter( pos );
		for ( auto& v : vertices )
		{
			v += moved;
		}
	}
	void SetSize( const Vec2<T>& size ) override
	{
		Collider<T>::SetSize( size );
		vertices[0]= Vec2<T>( this->rect.left, this->rect.top );
		vertices[1]= Vec2<T>( this->rect.right, this->rect.top );
		vertices[2]= Vec2<T>( this->rect.right, this->rect.bottom );
		vertices[3]= Vec2<T>( this->rect.left, this->rect.bottom );
	}
	void MoveBy( const Vec2<T>& offset ) override
	{
		Collider<T>::MoveBy( offset );
		for ( auto& v : vertices )
		{
			v += offset;
		}
	}
	std::vector<Vec2<T>> GetVertices() const override
	{
		return vertices;
	}
	void Draw( Gdiplus::Graphics& gfx, const Gdiplus::Color& color ) override
	{
		this->surf.DrawFillPolygonPlus( gfx, vertices, (int)vertices.size(), color );
	}

	Vec2<T> GetDir() override { return { (T)0,(T)0 }; }

private:
	std::vector<Vec2<T>> vertices;
};

template <typename T>
class CircleCollider : public Collider<T>
{
public:

	CircleCollider( const Vec2<T> topLeft, T radius )
		:
		circle( {topLeft.x + radius, topLeft.y + radius}, radius ),
		Collider<T>( Collider<T>::Type::Circle, { topLeft.y, topLeft.y + radius*2, topLeft.x, topLeft.x + radius* 2 } )
	{}

	void SetPos( const Vec2<T>& pos ) override
	{
		const Vec2<T> moved = pos - Collider<T>::rect.GetTopLeft();
		Collider<T>::SetPos( pos );
		circle.SetCenter( pos + Vec2<T>{circle.GetRadius(), circle.GetRadius()} );
	}
	void SetPosByCenter( const Vec2<T>& pos ) override
	{
		const Vec2<T> moved = pos - Collider<T>::rect.GetCenter();
		Collider<T>::SetPosByCenter( pos );
		circle.SetCenter( pos + Vec2<T>{circle.GetRadius(), circle.GetRadius()} );
	}

	void Draw( Gdiplus::Graphics& gfx, const Gdiplus::Color& color ) override
	{
		this->surf.DrawFillCirclePlus( gfx, circle.GetTopLeft(), circle.GetRadius(), color );
		auto testRect = circle.GetOuterRect();
		this->surf.DrawFillRectPlus( gfx, testRect.GetTopLeft(), testRect.GetWidth(), testRect.GetHeight(), { 144,0,0,255 } );
	}
	Vec2<T> GetDir() override { return { (T)0,(T)0 }; }
	
private:
	Circle<T> circle;
};

template <typename T>
class LineCollider : public Collider<T>
{
public:
	LineCollider( const Vec2<T>& startPos, const Vec2<T>& endPos )
		:
		line( startPos, endPos ),
		Collider<T>( Collider<T>::Type::Line,_Rect<T>{ startPos, endPos } )
	{}
	LineCollider( const Line<T>& line )
		:
		line(line),
		Collider<T>( Collider<T>::Type::Line, _Rect<T>{ line.GetStartPos(), line.GetEndPos() } )
	{}
	std::vector<Vec2<T>> GetVertices() const override
	{
		return std::vector<Vec2<T>>{ line.GetStartPos(), line.GetEndPos() };
	}
	void SetPos( const Vec2<T>& pos ) override
	{
		const Vec2<T> moved = pos - Collider<T>::rect.GetTopLeft();
		Collider<T>::SetPos( pos );
		line += moved;
	}
	void SetPosByCenter( const Vec2<T>& pos) override
	{
		const Vec2<T> moved = pos - Collider<T>::rect.GetCenter();
		Collider<T>::SetPosByCenter( pos );
		line += moved;
	}
	void Draw( Gdiplus::Graphics& gfx, const Gdiplus::Color& color ) override
	{
		//this->surf.DrawFillRectPlus( gfx, this->rect.GetTopLeft(), this->rect.GetWidth(), this->rect.GetHeight(), { 144,0,0,255 } );

		this->surf.DrawLinePlus( gfx, line.GetStartPos(), line.GetEndPos(), color, 1 );
	}

	Vec2<T> GetDir() override
	{
		return line.GetEndPos() - line.GetStartPos();
	}
private:
	Line<T> line;
};