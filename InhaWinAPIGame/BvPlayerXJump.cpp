#include "BvPlayerXJump.h"

void PlayerX::Jump::Activate( PlayerX& playerX, Scene& scene )
{
	playerX.SetAnimation( PlayerX::AnimationState::Jump, animSpeed );
	playerX.isJumpNow = true;
}

PlayerX::Behavior* PlayerX::Jump::Update( PlayerX& playerX, Scene& scene, float dt )
{
	if ( HasSucessors() )
	{
		return PassTorch();
	}

	DoJump( playerX, scene, dt, playerX.defaultMoveSpeed );

	return nullptr;
}

void PlayerX::Jump::DoJump( PlayerX& playerX, Scene& scene, float dt, float speed )
{
	if ( playerX.GetAnimation().IsEnd() )
	{
		playerX.curAnimation.SetStop();
	}

	playerX.vel.x = 0.0f;

	if ( !playerX.isOnWallSide )
	{
		if ( playerX.isRightKeyDown )
		{
			playerX.vel.x = speed;
		}
		if ( playerX.isLeftKeyDown )
		{
			playerX.vel.x = -speed;
		}
	}

	jumpTime += dt;
	if ( jumpTime <= jumpMaxTime )
	{
		playerX.vel.y = playerX.jumpSpeed;
		playerX.vel.y = playerX.gravity.GetGravityVel( playerX.vel, dt );
		playerX.Move( dt, scene );
	}
	else
	{
		playerX.isJumpNow = false;
		playerX.isJumpEnd = true;
	}
}

void PlayerX::DashJump::Activate( PlayerX& playerX, Scene& scene )
{
	playerX.SetAnimation( PlayerX::AnimationState::Jump, animSpeed );
	playerX.isJumpNow = true;
}

PlayerX::Behavior* PlayerX::DashJump::Update( PlayerX& playerX, Scene& scene, float dt )
{
	if ( HasSucessors() )
	{
		return PassTorch();
	}

	DoJump( playerX, scene, dt, dashSpeed );
	playerX.canAirDash = false;

	return nullptr;
}
