#pragma once

#include "Vec2.h"
#include "Rect.h"
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
	Collider( Type type, const Vec2<T>& pos )
		:
		type( type ),
		pos( pos )
	{
	}
	void UpdateMatrix( const Mat3<float>& transform )
	{
		surf.SetTransformation( transform );
	}
	Vec2<T> GetPos() const
	{
		return pos;
	}
	bool IsCollideWithAABB( const Collider<T>& other ) const;
	virtual bool IsCollideWithOBB( const Collider<T>& other ) const = 0;
	virtual std::vector<Vec2<T>> GetVertices() const = 0;
	virtual void Draw( Gdiplus::Graphics& gfx ) = 0;
	virtual void SetPos( const Vec2<T>& pos_in )
	{
		pos = pos_in;
	}
	virtual void MoveBy( const Vec2<T>& offset )
	{
		pos += offset;
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
		for ( int vIdx = 0; vIdx < refObjVertices.size(); ++vIdx )
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

	bool CheckCircleOverlap( const Collider<T>& other )
	{
		//const Vec2<float> distance = c1.GetCenter() - c2.GetCenter();
		//const float sumOfRadius = c1.GetRadius() + c2.GetRadius();
		//return fabs( distance.x * distance.x + distance.y * distance.y ) < sumOfRadius * sumOfRadius;
	}
	/*
	bool CheckConvexOverlapWithCircle( const Collider<T>& convex, const Collider<T>& circle ) const
	{
		//if ( CheckCircleOverlap( convex, circle ) )
		{
			const Vec2<T> convexVertices = convex.GetVertices();

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
				float p = (circle.GetCenter() + (normalizedAxis * circle.GetSize())) * axisProj;
				minOther = (std::min)(minOther, p);
				maxOther = (std::max)(maxOther, p);
				p = (circle.GetCenter() - (normalizedAxis * circle.GetSize())) * axisProj;
				minOther = (std::min)(minOther, p);
				maxOther = (std::max)(maxOther, p);

				if ( !(maxOther >= minThis && maxThis >= minOther) )
				{
					return false;
				}

			}
			return true;
		}
		return false;
	}
	*/
protected:
	Type type;
	Surface<T> surf;
	Vec2<T> pos;
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
		Collider<T>( Collider<T>::Type::Convex, rect.GetTopLeft() )
	{
		vertices.reserve( 4 );
		vertices.emplace_back( rect.left, rect.top );
		vertices.emplace_back( rect.right, rect.top );
		vertices.emplace_back( rect.right, rect.bottom );
		vertices.emplace_back( rect.left, rect.bottom );
	}
	ConvexCollider( const std::vector<Vec2<T>>& vertices, const Vec2<T>& pos )
		:
		Collider<T>( Collider<T>::Type::Convex, pos ),
		vertices( vertices )
	{}

	void SetPos( const Vec2<T>& pos ) override
	{
		const Vec2<T> moved = pos - Collider<T>::pos;
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
	std::vector<Vec2<T>> GetVertices() const override
	{
		return vertices;
	}
	void Draw( Gdiplus::Graphics& gfx ) override
	{
		this->surf.DrawFillPolygonPlus( gfx, vertices, (int)vertices.size(), this->debugColor );
	}

private:
	std::vector<Vec2<T>> vertices;
};