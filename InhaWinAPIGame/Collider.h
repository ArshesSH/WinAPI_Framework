#pragma once

#include "Vec2.h"
#include "Rect.h"
#include <vector>

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
	Collider(const Vec2<T>& pos, Type type)
		:
		pos(pos),
		type(type)
	{
	}
	void UpdatePos( const Vec2<T>& pos_in )
	{
		pos = pos_in;
	}

	virtual bool IsCollideWith( const Collider<T>& other ) const = 0;
	virtual const std::vector<Vec2<T>>& GetVertice() const = 0;

protected:
	bool CheckVerticesSAT( const Collider<T>& other ) const
	{
		const auto refObjVertices = GetVertice();
		const auto otherVertices = other.GetVertice();

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

	bool CheckConvexOverlapWitchCircle( const Collider& convex, const Collider& circle ) const
	{
		if ( CheckCircleOverlap( convex, circle ) )
		{
			const auto convexVertices = convex.GetVertices();

			for ( int vIdx = 0; vIdx < convexVertices.size(); ++vIdx )
			{
				const int vIdxNext = (vIdx + 1) % convexVertices.size();
				Vec2<float> axisProj = (convexVertices[vIdx] - convexVertices[vIdxNext]).GetNormalRightVec2().GetNormalized();

				float minThis = INFINITY;
				float maxThis = -INFINITY;
				for ( auto e : convexVertices )
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

protected:
	Vec2<T> pos;
	Type type;
};


template <typename T>
class ConvexCollider : Collider<T>
{
public:
	ConvexCollider(const Vec2<T>& pos, const _Rect<T>& rect)
		:
		Collider(pos, Collider<T>::Type::Convex )
	{
		vertices.reserve( 4 );
		vertices.emplace_back( rect.left, rect.top );
		vertices.emplace_back( rect.right, rect.top );
		vertices.emplace_back( rect.right, rect.bottom );
		vertices.emplace_back( rect.left, rect.bottom );
	}
	ConvexCollider( const Vec2<T>& pos, const std::vector<Vec2<T>>& vertices )
		:
		Collider( pos ),
		vertices( vertices )
	{}

	bool IsCollideWith( const Collider<T>& other ) const override
	{
		switch ( other.type )
		{
		case Collider<T>::Type::Convex:
			{
				this->CheckVerticesSAT( other );
			}
			break;
		case Collider<T>::Type::Circle:
			{
				this->CheckConvexOverlapWitchCircle( *this, other );
			}
			break;
		case Collider<T>::Type::Line:
			{
				this->CheckVerticesSAT( other );
			}
			break;
		}
	}
	const std::vector<Vec2<T>>& GetVertice() const override
	{
		return vertices;
	}

private:
	std::vector<Vec2<T>> vertices;
};