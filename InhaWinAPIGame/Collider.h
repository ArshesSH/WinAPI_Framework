#pragma once

#include "Vec2.h"
#include "Rect.h"
#include "Circle.h"
#include <vector>

#include "framework.h"
#include "Surface.h"

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
		type( type ),
		rect( rect )
	{
	}
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
	virtual bool IsOverlapWithAABB( const Collider<T>& other ) const
	{
		return rect.Overlaps( other.rect );
	}
	virtual bool IsOverlapWithOBB( const Collider<T>& other ) const = 0;
	virtual bool IsCollideWithOBB( const Collider<T>& other ) const = 0;
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
	virtual void MoveBy( const Vec2<T>& offset )
	{
		rect += offset;
	}
	Type GetType() const
	{
		return type;
	}
protected:
	bool CheckVerticesSAT( const Collider<T>& other ) const
	{
		const auto refObjVertices = GetVertices();
		const auto otherVertices = other.GetVertices();

		// Create Translate things
		float minTranslateScalar = INFINITY;
		Vec2<float> minTranslateNormalVec;

		// Check for each axis
		for ( int vIdx = 0; vIdx < (int)refObjVertices.size(); ++vIdx )
		{
			const int vIdxNext = (vIdx + 1) % refObjVertices.size();
			Vec2<float> axisProj = (refObjVertices[vIdx] - refObjVertices[vIdxNext]).GetNormalRightVec2().GetNormalized();

			float minThis = INFINITY;
			float maxThis = -INFINITY;
			for ( auto e : refObjVertices )
			{
				const float p = e * axisProj;
				minThis = (std::min)(minThis, p);
				maxThis = (std::max)(maxThis, p);
			}

			float minOther = INFINITY;
			float maxOther = -INFINITY;
			for ( auto e : otherVertices )
			{
				const float p = e * axisProj;
				minOther = (std::min)(minOther, p);
				maxOther = (std::max)(maxOther, p);
			}

			if ( !(maxOther >= minThis && maxThis >= minOther) )
			{
				return false;
			}
		}
		return true;
	}

	bool CheckCircleOverlap( const Collider<T>& other ) const
	{
		const Circle<T> thisCircle = GetCircle();
		const Circle<T> otherCircle = other.GetCircle();
		return thisCircle.IsOverlapWith( otherCircle );
	}
	bool CheckConvexOverlapWithCircle( const Collider<T>& convex, const Collider<T>& circle ) const
	{
		const auto convexVertices = convex.GetVertices();
		for ( int vIdx = 0; vIdx < convexVertices.size(); ++vIdx )
		{
			const int vIdxNext = (vIdx + 1) % convexVertices.size();
			Vec2<float> axisProj = (convexVertices[vIdx] - convexVertices[vIdxNext]).GetNormalRightVec2().GetNormalized();

			float minThis = INFINITY;
			float maxThis = -INFINITY;
			for ( Vec2<T> e : convexVertices )
			{
				const float p = e * axisProj;
				minThis = (std::min)(minThis, p);
				maxThis = (std::max)(maxThis, p);
			}

			float minOther = INFINITY;
			float maxOther = -INFINITY;

			const Vec2<float> normalizedAxis = axisProj.GetNormalized();
			const auto circleSize = circle.GetRect().GetWidth() / 2.0f;
			float p = (circle.GetCenter() + (normalizedAxis * circleSize)) * axisProj;
			minOther = (std::min)(minOther, p);
			maxOther = (std::max)(maxOther, p);
			p = (circle.GetCenter() - (normalizedAxis * circleSize)) * axisProj;
			minOther = (std::min)(minOther, p);
			maxOther = (std::max)(maxOther, p);

			if ( !(maxOther >= minThis && maxThis >= minOther) )
			{
				return false;
			}
		}
		return true;
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
	void MoveBy( const Vec2<T>& offset ) override
	{
		Collider<T>::MoveBy( offset );
		for ( auto& v : vertices )
		{
			v += offset;
		}
	}
	bool IsOverlapWithOBB( const Collider<T>& other ) const override
	{
		bool isCollide = false;
		switch ( other.GetType() )
		{
		case Collider<T>::Type::Convex:
			{
				isCollide = this->CheckVerticesSAT( other );
			}
			break;
		case Collider<T>::Type::Circle:
			{
				isCollide = this->CheckConvexOverlapWithCircle( *this, other );
			}
			break;
		case Collider<T>::Type::Line:
			{
				isCollide = this->CheckVerticesSAT( other );
			}
			break;
		}
		return isCollide;
	}
	bool IsCollideWithOBB( const Collider<T>& other ) const override
	{
		bool isCollide = false;
		switch ( other.GetType() )
		{
		case Collider<T>::Type::Convex:
			{
				isCollide = this->CheckVerticesSAT( other );
			}
			break;
		case Collider<T>::Type::Circle:
			{
				this->CheckConvexOverlapWithCircle( *this, other );
			}
			break;
		case Collider<T>::Type::Line:
			{
				isCollide = this->CheckVerticesSAT( other );
			}
			break;
		}
		return isCollide;
	}
	std::vector<Vec2<T>> GetVertices() const override
	{
		return vertices;
	}
	void Draw( Gdiplus::Graphics& gfx, const Gdiplus::Color& color ) override
	{
		this->surf.DrawFillPolygonPlus( gfx, vertices, (int)vertices.size(), color );
	}

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
	void Draw( Gdiplus::Graphics& gfx, const Gdiplus::Color& color ) override
	{
		this->surf.DrawFillCirclePlus( gfx, circle.GetTopLeft(), circle.GetRadius(), color );
		auto testRect = circle.GetOuterRect();
		this->surf.DrawFillRectPlus( gfx, testRect.GetTopLeft(), testRect.GetWidth(), testRect.GetHeight(), { 144,0,0,255 } );
	}
	bool IsOverlapWithOBB( const Collider<T>& other ) const override
	{
		bool isCollide = false;
		switch ( other.GetType() )
		{
		case Collider<T>::Type::Convex:
			{
				isCollide = this->CheckVerticesSAT( other );
			}
			break;
		case Collider<T>::Type::Circle:
			{
				//this->CheckConvexOverlapWitchCircle( other, *this );
			}
			break;
		case Collider<T>::Type::Line:
			{
				isCollide = this->CheckVerticesSAT( other );
			}
			break;
		}
		return isCollide;
	}

	bool IsCollideWithOBB( const Collider<T>& other ) const override
	{
		bool isCollide = false;
		switch ( other.GetType() )
		{
		case Collider<T>::Type::Convex:
			{
				isCollide = this->CheckVerticesSAT( other );
			}
			break;
		case Collider<T>::Type::Circle:
			{
				//this->CheckConvexOverlapWitchCircle( *this, other );
			}
			break;
		case Collider<T>::Type::Line:
			{
				isCollide = this->CheckVerticesSAT( other );
			}
			break;
		}
		return isCollide;
	}

private:
	Circle<T> circle;
};

template <typename T>
class LineCollider : public Collider<T>
{
public:
	LineCollider( const Vec2<T>& startPos, const Vec2<T>& endPos )
		:
		startPos(startPos),
		endPos(endPos),
		Collider<T>( startPos, endPos )
	{}
	void SetPos( const Vec2<T>& pos ) override
	{
		const Vec2<T> moved = pos - Collider<T>::rect.GetTopLeft();
		Collider<T>::SetPos( pos );
		startPos += pos;
		endPos += pos;
	}
	void Draw( Gdiplus::Graphics& gfx, const Gdiplus::Color& color ) override
	{
		auto testRect = this->GetOuterRect();
		this->surf.DrawFillRectPlus( gfx, testRect.GetTopLeft(), testRect.GetWidth(), testRect.GetHeight(), { 144,0,0,255 } );

		this->surf.DrawLinePlus( gfx, startPos, endPos, color, 1 );
	}


private:
	Vec2<T> startPos;
	Vec2<T> endPos;
};