#pragma once

#include "Collider.h"

template <typename T>
class CollisionManager
{
public:
	CollisionManager()
	{}

	bool IsOverlapWithAABB( const Collider<T>& ref, const Collider<T>& target ) const
	{
		if ( ref.GetType() == Collider<T>::Type::Convex )
		{
			if ( target.GetType() == Collider<T>::Type::Line )
			{
				return CheckConvexOverlapLine( ref, target );
			}
			else
			{
				return ref.GetRect().Overlaps( target.GetRect() );
			}
		}
		else if ( ref.GetType() == Collider<T>::Type::Circle )
		{
			if ( target.GetType() == Collider<T>::Type::Line )
			{
				return CheckConvexOverlapLine( ref, target );
			}
			else
			{
				return ref.GetRect().Overlaps( target.GetRect() );
			}
		}
		else
		{
			if ( target.GetType() == Collider<T>::Type::Line )
			{
				return CheckLineOverlapLine( ref, target );
			}
			else
			{
				return CheckConvexOverlapLine( target, ref );
			}
		}
	}

	bool IsOverlapWithOBB( const Collider<T>& ref, const Collider<T>& target ) const
	{
		if ( ref.GetType() == Collider<T>::Type::Convex )
		{
			if ( target.GetType() == Collider<T>::Type::Convex )
			{
				return CheckVerticesSAT( ref, target );
			}
			else if ( target.GetType() == Collider<T>::Type::Circle )
			{
				return CheckConvexOverlapCircle( ref, target );
			}
			else
			{
				return CheckConvexOverlapLine(ref, target);
			}
		}
		else if ( ref.GetType() == Collider<T>::Type::Circle )
		{
			if ( target.GetType() == Collider<T>::Type::Convex )
			{
				return CheckConvexOverlapCircle( target, ref );
			}
			else if ( target.GetType() == Collider<T>::Type::Circle )
			{
				return ref.GetCircle().IsOverlapWith( target.GetCircle() );
			}
			else
			{
				return CheckCircleOverlapLine( ref, target );
			}
		}
		else
		{
			if ( target.GetType() == Collider<T>::Type::Convex )
			{
				return CheckConvexOverlapLine( target, ref );
			}
			else if ( target.GetType() == Collider<T>::Type::Circle )
			{
				return CheckCircleOverlapLine( target, ref );
			}
			else
			{
				return CheckConvexOverlapLine( ref, target );
			}
		}
	}

private:
	bool CheckVerticesSAT( const Collider<T>& ref, const Collider<T>& other ) const
	{
		const std::vector<Vec2<T>>& refObjVertices = ref.GetVertices();
		const std::vector<Vec2<T>>& otherVertices = other.GetVertices();

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
		const Vec2<T> circlePos = circle.GetCenter();
		const auto curLineEndPos = curLine.GetEndPos();
		const auto curLineStartPos = curLine.GetStartPos();

		const float dx = curLineEndPos.x - curLineStartPos.x;
		const float dy = curLineEndPos.y - curLineStartPos.y;
		const float a = dx * dx + dy * dy;
		const float b = 2 * (dx * (curLineStartPos.x - circlePos.x) + dy * (curLineStartPos.y - circlePos.y));
		float c = circlePos.x * circlePos.x + circlePos.y * circlePos.y;
		c += curLineStartPos.x * curLineStartPos.x + curLineStartPos.y * curLineStartPos.y;
		c -= 2 * (circlePos.x * curLineStartPos.x + circlePos.y * curLineStartPos.y);
		c -= circle.GetRadius() * circle.GetRadius();

		return b * b - 4 * a * c >= 0;
	}

private:
	bool isCollide = false;
};