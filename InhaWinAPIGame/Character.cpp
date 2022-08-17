#include "Character.h"
#include "Scene.h"

bool Character::IsCollideWithWall( const Vec2<float>& nextPos, const Scene& scene ) const
{
	const ConvexCollider<float> nextCollider( RectF::FromCenter( nextPos, colliderHalfWidth, colliderHalfHeight ) );
	const auto walls = scene.GetWallPtrs();
	for ( const auto& wall : walls )
	{
		if ( wall->IsCollideWith( nextCollider ) )
		{
			return false;
		}
	}
	return true;
}

void Character::Move( float dt, const Scene& scene )
{
	const Vec2<float> nextPos = GetPos() + dir * moveSpeed * dt;

	if ( !IsCollideWithWall( nextPos, scene ) )
	{
		SetPos( nextPos );
	}
}
