#include "CollisionManager.h"

void CollisionManager::CircleToCircle( PhysicsEntity& circle1, PhysicsEntity& circle2 )
{
	if ( CheckCircleOverlap( circle1, circle2 ) )
	{
		// Displace this and other
		const Vec2<float> distVec = circle1.GetCenter() - circle2.GetCenter();
		const float distance = distVec.GetLength();
		const float ovelapDist = (distance - circle1.GetSize() - circle2.GetSize()) * 0.5f;
		const Vec2<float> distOverlapVec = distVec.GetNormalized() * ovelapDist;

		CenterCorrection( circle1, distOverlapVec * -1.0f );
		CenterCorrection( circle2, distOverlapVec );

		SwapVelocity( circle1, circle2 );

		circle1.SetState( PhysicsEntity::State::Collided );
		circle2.SetState( PhysicsEntity::State::Collided );
	}
}

void CollisionManager::ConvexToConvex( PhysicsEntity& convex1, PhysicsEntity& convex2 )
{
	Vec2<float> minTranslateVecConvex1;
	Vec2<float> minTranslateVecConvex2;

	if ( CheckConvexOverlapWithConvex( convex1, convex2, minTranslateVecConvex1, minTranslateVecConvex2 ) )
	{
		// Set Center Correction
		CenterCorrection( convex1, minTranslateVecConvex1 );
		CenterCorrection( convex2, minTranslateVecConvex2 );

		SwapVelocity( convex1, convex2 );

		SetCollisionStateBoth( convex1, convex2 );
	}
}

void CollisionManager::ConvexToCircle( PhysicsEntity& convex, PhysicsEntity& circle )
{
	Vec2<float> minTranslateVec;
	if ( CheckConvexOverlapWitchCircle( convex, circle, minTranslateVec ) )
	{
		CenterCorrection( convex, minTranslateVec );
		CenterCorrection( circle, minTranslateVec * -1.0f );

		SwapVelocity( convex, circle );

		SetCollisionStateBoth( convex, circle );
	}
}

void CollisionManager::DoWallCollision( PhysicsEntity& entity, const RECT& walls )
{
	if ( entity.GetType() == PhysicsEntity::Type::Rect )
	{
		const Vec2<float> topLeftVec{ (float)walls.left, (float)walls.top };
		const Vec2<float> bottomRightVec{ (float)walls.right, (float)walls.bottom };

		if ( CheckConvexOverlapWithborder( entity, topLeftVec, bottomRightVec ) )
		{
		}
	}
	else
	{
		const RECT objRect = entity.GetRECT();
		if ( objRect.left < walls.left )
		{
			entity.SetCenterX( entity.GetCenterX() + walls.left - objRect.left );
			entity.ReboundX();
		}
		else if ( objRect.right > walls.right )
		{
			entity.SetCenterX( entity.GetCenterX() + (walls.right - objRect.right) );
			entity.ReboundX();
		}
		if ( objRect.top < walls.top )
		{
			entity.SetCenterY( entity.GetCenterY() + walls.top - objRect.top );
			entity.ReboundY();
		}
		else if ( objRect.bottom > walls.bottom )
		{
			entity.SetCenterY( entity.GetCenterY() + (walls.bottom - objRect.bottom) );
			entity.ReboundY();
		}
	}
}

bool CollisionManager::DoConvexOverlapWithborder( PhysicsEntity& entity, const Vec2<float>& topLeft, const Vec2<float>& bottomRight )
{
	// Create Normalized Horizontal and Vertical Window Sized Vectors
	const Vec2<float> NormalizedHorizontal = Vec2<float>( bottomRight.x - topLeft.x, topLeft.y ).GetNormalized();
	const Vec2<float> NormalizedVertical = Vec2<float>( topLeft.x, bottomRight.y - topLeft.y ).GetNormalized();

	// Set Projection vals
	float minHorizon = INFINITY;
	float maxHorizon = -INFINITY;
	float minVertical = INFINITY;
	float maxVertical = -INFINITY;

	std::vector<Vec2<float>> vertices = entity.GetVertices();

	for ( auto& e : vertices )
	{
		// Check Horizontal
		const float pHorizon = e * NormalizedHorizontal;
		minHorizon = (std::min)(minHorizon, pHorizon);
		maxHorizon = (std::max)(maxHorizon, pHorizon);

		// Case Left Collision
		if ( minHorizon < topLeft.x )
		{
			const Vec2<float> minimumTranslateVec = NormalizedHorizontal * (topLeft.x - e.x);
			entity.SetCenter( entity.GetCenter() + minimumTranslateVec );
			entity.ReboundX();
			return true;
		}
		// Case Right Collision
		else if ( bottomRight.x < maxHorizon )
		{
			const Vec2<float> minimumTranslateVec = NormalizedHorizontal * (bottomRight.x - e.x);
			entity.SetCenter( entity.GetCenter() + minimumTranslateVec );
			entity.ReboundX();
			return true;
		}

		// Check Vertical
		const float pVertical = e * NormalizedVertical;
		minVertical = (std::min)(minVertical, pVertical);
		maxVertical = (std::max)(maxVertical, pVertical);

		// Case Top Collision
		if ( minVertical < topLeft.y )
		{
			const Vec2<float> minimumTranslateVec = NormalizedVertical * (topLeft.y - e.y);
			entity.SetCenter( entity.GetCenter() + minimumTranslateVec );
			entity.ReboundY();
			return true;
		}
		// Case Bottom Collision
		else if ( bottomRight.y < maxVertical )
		{
			const Vec2<float> minimumTranslateVec = NormalizedVertical * (bottomRight.y - e.y);
			entity.SetCenter( entity.GetCenter() + minimumTranslateVec );
			entity.ReboundY();
			return true;
		}
	}
	return false;
}

bool CollisionManager::CheckCircleOverlap( const PhysicsEntity& e1, const PhysicsEntity& e2 )
{
	Circle<float> c1( e1.GetCenter(), e1.GetOuterRadius() );
	Circle<float> c2( e2.GetCenter(), e2.GetOuterRadius() );

	const Vec2<float> distance = c1.GetCenter() - c2.GetCenter();
	const float sumOfRadius = c1.GetRadius() + c2.GetRadius();
	return fabs( distance.x * distance.x + distance.y * distance.y ) < sumOfRadius * sumOfRadius;
}

bool CollisionManager::CheckConvexOverlapWithConvex( PhysicsEntity& convex1, PhysicsEntity& convex2, Vec2<float>& minTransVec1, Vec2<float>& minTransVec2 )
{
	// First, Check Collision with Outer Circles
	if ( CheckCircleOverlap( convex1, convex2 ) )
	{
		Vec2<float> minTranslateVecConvex1;
		Vec2<float> minTranslateVecConvex2;
		// Then, Do OBB Collision detect for convex1 and convex2
		if ( CheckVerticesSAT( convex1, convex2, minTranslateVecConvex1 ) == false )
		{
			return false;
		}
		if ( CheckVerticesSAT( convex2, convex1, minTranslateVecConvex2 ) == false )
		{
			return false;
		}

		minTransVec1 = minTranslateVecConvex1;
		minTransVec2 = minTranslateVecConvex2;

		return true;
	}
	return false;
}

bool CollisionManager::CheckConvexOverlapWitchCircle( PhysicsEntity& convex, PhysicsEntity& circle, Vec2<float>& minTransVec )
{
	if ( CheckCircleOverlap( convex, circle ) )
	{
		const auto convexVertices = convex.GetVertices();

		// Create Translate things
		float minTranslateScalar = INFINITY;
		Vec2<float> minTranslateNormalVec;

		for ( int vIdx = 0; vIdx < (int)convexVertices.size(); ++vIdx )
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

			// Get MinTranslate Scalr and Vector
			const float curMinTrans = maxOther - minThis;
			if ( curMinTrans < minTranslateScalar )
			{
				minTranslateScalar = curMinTrans;
				minTranslateNormalVec = axisProj;
			}
		}
		minTransVec = minTranslateNormalVec * (minTranslateScalar * 0.5f);
		return true;
	}
	return false;
}

bool CollisionManager::CheckConvexOverlapWithborder( PhysicsEntity& entity, const Vec2<float>& topLeft, const Vec2<float>& bottomRight )
{
	// Create Normalized Horizontal and Vertical Window Sized Vectors
	const Vec2<float> NormalizedHorizontal = Vec2<float>( bottomRight.x - topLeft.x, topLeft.y ).GetNormalized();
	const Vec2<float> NormalizedVertical = Vec2<float>( topLeft.x, bottomRight.y - topLeft.y ).GetNormalized();

	// Set Projection vals
	float minHorizon = INFINITY;
	float maxHorizon = -INFINITY;
	float minVertical = INFINITY;
	float maxVertical = -INFINITY;

	std::vector<Vec2<float>> vertices = entity.GetVertices();

	for ( auto& e : vertices )
	{
		// Check Horizontal
		const float pHorizon = e * NormalizedHorizontal;
		minHorizon = (std::min)(minHorizon, pHorizon);
		maxHorizon = (std::max)(maxHorizon, pHorizon);

		// Case Left Collision
		if ( minHorizon < topLeft.x )
		{
			const Vec2<float> minimumTranslateVec = NormalizedHorizontal * (topLeft.x - e.x);
			return true;
		}
		// Case Right Collision
		else if ( bottomRight.x < maxHorizon )
		{
			const Vec2<float> minimumTranslateVec = NormalizedHorizontal * (bottomRight.x - e.x);
			return true;
		}

		// Check Vertical
		const float pVertical = e * NormalizedVertical;
		minVertical = (std::min)(minVertical, pVertical);
		maxVertical = (std::max)(maxVertical, pVertical);

		// Case Top Collision
		if ( minVertical < topLeft.y )
		{
			const Vec2<float> minimumTranslateVec = NormalizedVertical * (topLeft.y - e.y);
			return true;
		}
		// Case Bottom Collision
		else if ( bottomRight.y < maxVertical )
		{
			const Vec2<float> minimumTranslateVec = NormalizedVertical * (bottomRight.y - e.y);
			return true;
		}
	}
	return false;
}

inline bool CollisionManager::CheckVerticesSAT( const PhysicsEntity& refObj, const PhysicsEntity& target, Vec2<float>& minTransVec )
{
	auto refObjVertices = refObj.GetVertices();
	auto refObjVerticesSize = refObjVertices.size();
	auto targetVertices = target.GetVertices();

	// Create Translate things
	float minTranslateScalar = INFINITY;
	Vec2<float> minTranslateNormalVec;

	// Check for each axis
	for ( int vIdx = 0; vIdx < (int)refObjVerticesSize; ++vIdx )
	{
		const int vIdxNext = (vIdx + 1) % refObjVerticesSize;
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
		for ( auto e : targetVertices )
		{
			const float p = e * axisProj;
			minOther = (std::min)(minOther, p);
			maxOther = (std::max)(maxOther, p);
		}

		if ( !(maxOther >= minThis && maxThis >= minOther) )
		{
			return false;
		}

		// Get MinTranslate Scalr and Vector
		const float curMinTrans = maxOther - minThis;
		if ( curMinTrans < minTranslateScalar )
		{
			minTranslateScalar = curMinTrans;
			minTranslateNormalVec = axisProj;
		}
	}

	// return minimum translate Vector
	minTransVec = minTranslateNormalVec * (minTranslateScalar * 0.5);
	return true;
}

inline void CollisionManager::CenterCorrection( PhysicsEntity& entity, const Vec2<float>& correctionVec )
{
	entity.SetCenter( entity.GetCenter() + correctionVec );
}

inline void CollisionManager::SwapVelocity( PhysicsEntity& e1, PhysicsEntity& e2 )
{
	auto tmpVel = e1.GetVelocity();
	e1.SetVelocity( e2.GetVelocity() );
	e2.SetVelocity( tmpVel );
}

inline void CollisionManager::SetCollisionStateBoth( PhysicsEntity& e1, PhysicsEntity& e2 )
{
	e1.SetState( PhysicsEntity::State::Collided );
	e2.SetState( PhysicsEntity::State::Collided );
}
