#include "Character.h"
#include "Scene.h"

#ifndef NDBUG
#include <iostream>
#endif // !NDBUG



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

bool Character::IsCollideWithGround( const Vec2<float>& nextPos, const Scene& scene ) const
{
	const ConvexCollider<float> nextCollider( RectF::FromCenter( nextPos, colliderHalfWidth, colliderHalfHeight ) );
	const auto& grounds = scene.GetGroundPtrs();
	for ( const auto& ground : grounds )
	{
		if ( ground->IsCollideWith( scene.GetCollisionManager(), nextCollider ) )
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


	SetPos( nextPos );

	if ( IsCollideWithWall( nextColliderPos, scene ) )
	{
		SetPos( nextPos - moveAmount );
	}
	const ConvexCollider<float> nextCollider( RectF::FromCenter( nextPos, colliderHalfWidth, colliderHalfHeight ) );
	const auto& grounds = scene.GetGroundPtrs();
	for ( const auto& ground : grounds )
	{
		if ( ground->IsCollideWith( scene.GetCollisionManager(), nextCollider ) )
		{
			//auto groundDir = ground->GetColliderPtr()->GetDir().GetNormalized();
			//groundDir = (isFacingRight) ? groundDir : -groundDir;
			//const auto dotVel = groundDir * (vel.GetNormalized() * groundDir);
			//SetPos( nextPos + dotVel* dt);

			auto groundDir = ground->GetColliderPtr()->GetDir();

			float gradiant = 0.0f;
			if ( groundDir.x != 0.0f )
			{
				gradiant = groundDir.y / groundDir.x;
			}

			SetPos( {nextPos.x, nextPos.y + gradiant * moveAmount.x} );
		}
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

