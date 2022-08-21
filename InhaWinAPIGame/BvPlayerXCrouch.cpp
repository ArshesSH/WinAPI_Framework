#include "BvPlayerXCrouch.h"

void PlayerX::Crouch::Activate( PlayerX& playerX, Scene& scene )
{
	playerX.SetAnimation( PlayerX::AnimationState::Crouch, animSpeed );
	playerX.vel = { 0.0f, 0.0f };
}

PlayerX::Behavior* PlayerX::Crouch::Update( PlayerX& playerX, Scene& scene, float dt )
{
	if ( HasSucessors() )
	{
		return PassTorch();
	}

	int dirChange = 0;

	if ( playerX.isLeftKeyDown )
	{
		dirChange--;
	}
	if ( playerX.isRightKeyDown )
	{
		dirChange++;
	}

	if ( dirChange > 0 )
	{
		playerX.isFacingRight = true;
	}
	else if ( dirChange < 0 )
	{
		playerX.isFacingRight = false;
	}


	if ( playerX.curAnimation.IsEnd() )
	{
		playerX.curAnimation.SetStop();
	}


	return nullptr;
}
