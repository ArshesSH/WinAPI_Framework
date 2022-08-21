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

	if ( playerX.GetAnimation().IsEnd() )
	{
		playerX.curAnimation.SetStop();
	}

	SetVelXByInput( playerX, playerX.defaultMoveSpeed );
	DoJump( playerX, scene, dt );

	return nullptr;
}

void PlayerX::Jump::SetVelXByInput( PlayerX& playerX, float speed )
{
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
}

void PlayerX::Jump::DoJump( PlayerX& playerX, Scene& scene, float dt )
{
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

	if ( playerX.GetAnimation().IsEnd() )
	{
		playerX.curAnimation.SetStop();
	}
	SetVelXByInput( playerX, playerX.dashSpeed );
	DoJump( playerX, scene, dt );
	playerX.canAirDash = false;

	return nullptr;
}

void PlayerX::WallKick::Activate( PlayerX& playerX, Scene& scene )
{
	playerX.SetAnimation( PlayerX::AnimationState::WallKick, animSpeed );
	playerX.isJumpNow = true;
	playerX.hoverCount = 2;
}

PlayerX::Behavior* PlayerX::WallKick::Update( PlayerX& playerX, Scene& scene, float dt )
{
	float changedVel = 0.0f;
	if ( playerX.isFacingRight )
	{
		changedVel = (playerX.isZKeyDown) ? -playerX.dashSpeed : -playerX.defaultMoveSpeed;
		playerX.vel.x = changedVel;
	}
	else
	{
		changedVel = (playerX.isZKeyDown) ? playerX.dashSpeed : playerX.defaultMoveSpeed;
		playerX.vel.x = changedVel;
	}

	DoJump( playerX, scene, dt );

	if ( playerX.GetAnimation().IsEnd() )
	{
		playerX.moveState = MoveState::Airbone;
		playerX.UpdatePlayerBehavior();
		playerX.SetStopFacingTrack( false );
		return PassTorch();
	}


	return nullptr;
}
