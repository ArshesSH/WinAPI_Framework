#include "BvPlayerXAirbone.h"

void PlayerX::Airbone::Activate( PlayerX& playerX, Scene& scene )
{
	playerX.SetAnimation( PlayerX::AnimationState::Airbone, 0.0f );
}

PlayerX::Behavior* PlayerX::Airbone::Update( PlayerX& playerX, Scene& scene, float dt )
{
	if ( HasSucessors() )
	{
		return PassTorch();
	}

	playerX.vel.y -= 10.0f * dt;
	playerX.Move( dt, scene );

	return nullptr;
}
