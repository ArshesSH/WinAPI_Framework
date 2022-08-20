#include "BvPlayerXAirbone.h"

void PlayerX::Airbone::Activate( PlayerX& playerX, Scene& scene )
{
	playerX.SetAnimation( PlayerX::AnimationState::Airbone, 0.0f );
}

PlayerX::Behavior* PlayerX::Airbone::Update( PlayerX& playerX, Scene& scene, float dt )
{
	if ( HasSucessors() )
	{
		playerX.vel.y = 0.0f;
		return PassTorch();
	}

	playerX.vel.y = playerX.gravity.GetGravityVel( playerX.vel, dt );
	if ( playerX.isZKeyDown )
	{
		if ( playerX.isRightKeyDown )
		{
			playerX.vel.x = playerX.dashSpeed;
		}
		if ( playerX.isLeftKeyDown )
		{
			playerX.vel.x = -playerX.dashSpeed;
		}
	}
	else
	{
		if ( playerX.isRightKeyDown )
		{
			playerX.vel.x = playerX.defaultMoveSpeed;
		}
		if ( playerX.isLeftKeyDown )
		{
			playerX.vel.x = -playerX.defaultMoveSpeed;
		}
	}



	playerX.Move( dt, scene );

	return nullptr;
}
