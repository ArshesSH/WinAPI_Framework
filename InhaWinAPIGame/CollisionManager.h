#pragma once

#include "Collider.h"
#include "ColliderTypeTrait.h"
#include "CollisionPatternMatching.h"

template <typename T>
class CollisionManager
{
public:
	CollisionManager()
	{
		overlapAABBSwitch.Case<TypeConvex<T>, TypeConvex<T>>( [&]( Collider<T>& convex1, Collider<T>& convex2 )
			{
				if ( convex1.GetRect().Overlaps( convex2.GetRect() ) )
				{
					convex1.SetCollided();
					convex2.SetCollided();
				}
			}
		);
		/*
		overlapAABBSwitch.Case<Convex, Circle>( [&]( Collider<T>& convex1, Collider<T>& convex2 )
			{
				convex1.GetRect().Overlaps( convex2.GetRect() );
			}
		);
		overlapAABBSwitch.Case<Circle, Circle>( [&]( Collider<T>& convex1, Collider<T>& convex2 )
			{
				convex1.GetRect().Overlaps( convex2.GetRect() );
			}
		);
		overlapAABBSwitch.Case<Convex, Line>( [&]( Collider<T>& convex, Collider<T>& line )
			{
				CheckConvexOverlapLine( convex, line );
			}
		);
		overlapAABBSwitch.Case<Circle, Line>( [&]( Collider<T>& circle, Collider<T>& line )
			{
				CheckCircleOverlapLine( circle, line );
			}
		);
		overlapAABBSwitch.Case<Line, Line>( [&]( Collider<T>& line1, Collider<T>& line2 )
			{
				CheckLineOverlapLine( line1, line2 );
			}
		);*/
	}

	bool IsOverlapWithAABB( Collider<T>& ref, Collider<T>& target )
	{


		if ( overlapAABBSwitch.HasCase<TypeConvex<T>, TypeConvex<T>>() )
		{
			overlapAABBSwitch.Switch( ref, target );
			return ref.GetCollide();
		}
		return isCollide;


	}
private:
	bool CheckVerticesSAT( const std::vector<Vec2<T>>& refObjVertices, const std::vector<Vec2<T>>& otherVertices ) const
	{
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

	bool CheckConvexOverlapCircle( const Collider<T>& convex, const Collider<T>& circle ) const
	{
		const auto convexVertices = convex.GetVertices();
		for ( int vIdx = 0; vIdx < (int)convexVertices.size(); ++vIdx )
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

	bool CheckLineOverlapLine( const Collider<T>& lineCollider1, const Collider<T>& lineCollider2 ) const
	{
		const auto lineVertices1 = lineCollider1.GetVertices();
		const auto lineVertices2 = lineCollider2.GetVertices();
		const Line<T> line1 = { lineVertices1[0], lineVertices1[1] };
		const Line<T> line2 = { lineVertices2[0], lineVertices2[1] };

		return line1.IsOverlapWith( line2 );
	}
	bool CheckConvexOverlapLine( const Collider<T>& convex, const Collider<T>& line ) const
	{
		const auto convexVertices = convex.GetVertices();
		const auto lineVertices = line.GetVertices();
		const Line<T> curLine = { lineVertices[0], lineVertices[1] };

		for ( int vIdx = 0; vIdx < (int)convexVertices.size(); ++vIdx )
		{
			const int vIdxNext = (vIdx + 1) % convexVertices.size();
			const Line<T> convexLine = { convexVertices[vIdx], convexVertices[vIdxNext] };
			if ( curLine.IsOverlapWith( convexLine ) )
			{
				return true;
			}
		}
		return false;
	}
	bool CheckCircleOverlapLine( const Collider<T>& circleCollider, const Collider<T>& line ) const
	{
		const auto lineVertices = line.GetVertices();
		const Line<T> curLine = { lineVertices[0], lineVertices[1] };
		const Circle<T> circle = circleCollider.GetCircle();
		const Vec2<T> circlePos = circle.center;

		const float dx = curLine.endPos.x - curLine.startPos.x;
		const float dy = curLine.endPos.y - curLine.startPos.y;
		const float a = dx * dx + dy * dy;
		const float b = 2 * (dx * (curLine.startPos.x - circlePos.x) + dy * (curLine.startPos.y - circlePos.y));
		float c = circlePos.x * circlePos.x + circlePos.y * circlePos.y;
		c += curLine.startPos.x * curLine.startPos.x + curLine.startPos.y * curLine.startPos.y;
		c -= 2 * (circlePos.x * curLine.startPos.x + circlePos.y * curLine.startPos.y);
		c -= circle.GetRadius() * circle.GetRadius();

		return b * b - 4 * a * c >= 0;
	}

private:
	CollisionPatternMatching<T> overlapAABBSwitch;
	bool isCollide = false;
};