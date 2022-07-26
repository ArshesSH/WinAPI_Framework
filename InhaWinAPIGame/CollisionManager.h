#pragma once

#include "PhysicsEntity.h"

class CollisionManager
{
public:
	void CircleToCircle( PhysicsEntity& circle1, PhysicsEntity& circle2 );
	void ConvexToConvex( PhysicsEntity& convex1, PhysicsEntity& convex2 );
	void ConvexToCircle( PhysicsEntity& convex, PhysicsEntity& circle );
	void DoWallCollision( PhysicsEntity& entity, const RECT& walls );
	bool DoConvexOverlapWithborder( PhysicsEntity& entity, const Vec2<float>& topLeft, const Vec2<float>& bottomRight );

	bool CheckCircleOverlap( const PhysicsEntity& e1, const PhysicsEntity& e2 );
	bool CheckConvexOverlapWithConvex( PhysicsEntity& convex1, PhysicsEntity& convex2,
		Vec2<float>& minTransVec1, Vec2<float>& minTransVec2 );
	bool CheckConvexOverlapWitchCircle( PhysicsEntity& convex, PhysicsEntity& circle, Vec2<float>& minTransVec );
	bool CheckConvexOverlapWithborder( PhysicsEntity& entity, const Vec2<float>& topLeft, const Vec2<float>& bottomRight );

private:
	inline bool CheckVerticesSAT( const PhysicsEntity& refObj, const PhysicsEntity& target, Vec2<float>& minTransVec );
	inline void CenterCorrection( PhysicsEntity& entity, const Vec2<float>& correctionVec );
	inline void SwapVelocity( PhysicsEntity& e1, PhysicsEntity& e2 );
	inline void SetCollisionStateBoth( PhysicsEntity& e1, PhysicsEntity& e2 );
	
};