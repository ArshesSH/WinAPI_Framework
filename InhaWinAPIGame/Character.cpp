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
	const Vec2<float> nextPos = GetPos() + vel * dt;

	if ( !IsCollideWithWall( nextPos, scene ) )
	{
		SetPos( nextPos );
	}
}

