#include "Bullet.h"
#include "Scene.h"

void Bullet::Update( float dt, Scene& scene )
{
	Move( dt, scene );

	const auto& enemies = scene.FindActorByTag( oppositeTag );
	for ( const auto& enemy : enemies )
	{
		if ( enemy->IsCollideWith( scene.GetCollisionManager(), *(this->GetColliderPtr()) ) )
		{
			
		}
	}

	if ( IsCollideWithWall( GetNextPos( dt ), scene ) )
	{
		SetDestroy();
	}
}
