#include "BvPlayerXWallSlide.h"

void PlayerX::WallSlide::Activate( PlayerX& playerX, Scene& scene )
{
	playerX.SetAnimation( PlayerX::AnimationState::WallCling, animStartSpeed );
	playerX.vel = { 0.0f, wallSlideSpeed };
	playerX.SetStopFacingTrack();
	playerX.hoverCount = 0;
}

PlayerX::Behavior* PlayerX::WallSlide::Update( PlayerX& playerX, Scene& scene, float dt )
{
	if ( HasSucessors() )
	{
		playerX.SetStopFacingTrack(false);
		playerX.vel.y = 0.0f;
		return PassTorch();
	}


	if ( !(GetAsyncKeyState( 'X' ) & 0x8001) )
	{
		checkReinput = true;
	}
	if ( checkReinput )
	{
		playerX.vel.x = 0.0f;
		if ( playerX.isXKeyDown )
		{
			playerX.moveState = MoveState::WallKick;
			playerX.UpdatePlayerBehavior();
			return PassTorch();
		}
	}

	if ( playerX.curAnimState == AnimationState::WallCling && playerX.curAnimation.IsEnd() )
	{
		playerX.SetAnimation( AnimationState::WallSlide, 0.0f );
	}

	if (( playerX.isFacingRight && playerX.isLeftKeyDown )|| (!playerX.isFacingRight && playerX.isRightKeyDown ))
	{
		playerX.moveState = MoveState::Airbone;
		playerX.UpdatePlayerBehavior();
	}
	playerX.Move( dt, scene );

	return nullptr;
}
