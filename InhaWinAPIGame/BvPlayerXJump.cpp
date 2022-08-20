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

	playerX.vel.x = 0.0f;
	if ( playerX.isRightKeyDown )
	{
		playerX.vel.x = moveSpeed;
	}
	if ( playerX.isLeftKeyDown )
	{
		playerX.vel.x = -moveSpeed;
	}

	jumpTime += dt;
	if ( jumpTime <= jumpMaxTime )
	{
		playerX.vel.y = jumpSpeed;
		playerX.vel.y = playerX.gravity.GetGravityVel( playerX.vel, dt );
		playerX.Move( dt, scene );
	}
	else
	{
		playerX.isJumpNow = false;
	}

	return nullptr;
}
