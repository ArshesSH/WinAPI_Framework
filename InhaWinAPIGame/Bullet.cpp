#include "Bullet.h"
#include "Scene.h"

void Bullet::Update( float dt, Scene& scene )
{
	time += dt;

	if ( time >= lifeCycle )
	{
		SetDestroy();
	}

	Move( dt, scene );

	const auto& enemies = scene.FindActorByTag( oppositeTag );
	for ( const auto& enemy : enemies )
	{
		if ( enemy->IsCollideWith( scene.GetCollisionManager(), *(this->GetColliderPtr()) ) )
		{
			enemy->ApplyDamage( damage );
		}
	}

	if ( IsCollideWithWall( GetNextPos( dt ), scene ) || IsCollideWithGround( GetNextPos( dt ), scene ) )
	{
		SetDestroy();
	}
	
}
