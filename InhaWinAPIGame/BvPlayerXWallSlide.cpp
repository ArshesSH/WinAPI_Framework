#include "BvPlayerXWallSlide.h"

void PlayerX::WallSlide::Activate( PlayerX& playerX, Scene& scene )
{
	playerX.SetAnimation( PlayerX::AnimationState::WallSlide, 0.0f );
	playerX.vel = { 0.0f, wallSlideSpeed };
	playerX.SetStopFacingTrack();
}

PlayerX::Behavior* PlayerX::WallSlide::Update( PlayerX& playerX, Scene& scene, float dt )
{
	if ( HasSucessors() )
	{
		playerX.SetStopFacingTrack(false);
		playerX.vel.y = 0.0f;
		return PassTorch();
	}

	if (( playerX.isFacingRight && playerX.isLeftKeyDown )|| (!playerX.isFacingRight && playerX.isRightKeyDown ))
	{
		playerX.moveState = MoveState::Airbone;
		playerX.UpdatePlayerBehavior();
	}
	playerX.Move( dt, scene );

	return nullptr;
}