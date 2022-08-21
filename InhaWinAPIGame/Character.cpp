#include "Character.h"
#include "Scene.h"

bool Character::IsCollideWithWall( const Vec2<float>& nextPos, const Scene& scene ) const
{
	const ConvexCollider<float> nextCollider( RectF::FromCenter( nextPos, colliderHalfWidth, colliderHalfHeight ) );
	const auto& walls = scene.GetWallPtrs();
	for ( const auto& wall : walls )
	{
		if ( wall->IsCollideWith( scene.GetCollisionManager(), nextCollider ) )
		{
			return true;
		}
	}
	return false;
}



void Character::Move( float dt, const Scene& scene )
{
	const auto moveAmount = vel * dt;
	const Vec2<float> nextPos = GetPos() + moveAmount;
	const Vec2<float> nextColliderPos = GetColliderPos() + vel * dt;

	//if ( !IsCollideWithWall( nextColliderPos, scene ) )
	{
		SetPos( nextPos );
	}

	if ( IsCollideWithWall( nextColliderPos, scene ) )
	{
		SetPos( nextPos - moveAmount );
	}
}

Vec2<float> Character::GetNextPos( float dt ) const
{
	return GetPos() + vel * dt;
}

Vec2<float> Character::GetNextColliderPos( float dt ) const
{
	return  GetColliderPos() + vel * dt;
}

