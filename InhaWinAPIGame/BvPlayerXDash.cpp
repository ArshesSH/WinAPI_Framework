#include "BvPlayerXDash.h"
#include "BvPlayerXIdle.h"

void PlayerX::Dash::Activate( PlayerX& playerX, Scene& scene )
{
	playerX.SetAnimation( PlayerX::AnimationState::DashStart, animStartSpeed );
}

PlayerX::Behavior* PlayerX::Dash::Update( PlayerX& playerX, Scene& scene, float dt )
{
	if ( HasSucessors() )
	{
		return PassTorch();
	}
	if ( playerX.curAnimState == AnimationState::DashStart && playerX.curAnimation.IsEnd() )
	{
		playerX.SetAnimation( AnimationState::DashLoop, animLoopSpeed );
	}
	if ( isDashEnd )
	{
		StopDash(playerX, dt);
	}

	DoDash( playerX, scene, dt );

	return nullptr;
}

void PlayerX::Dash::DoDash( PlayerX& playerX, Scene& scene, float dt )
{
	dashTime += dt;

	if ( dashTime <= dashMaxTime )
	{
		if ( playerX.isRightKeyDown )
		{
			playerX.vel.x = moveSpeed;
		}
		if ( playerX.isLeftKeyDown )
		{
			playerX.vel.x = -moveSpeed;
		}
		playerX.Move( dt, scene );
	}
	else
	{
		StopDash(playerX, dt);
	}
}

void PlayerX::Dash::StopDash( PlayerX& playerX, float dt )
{
	//playerX.pBehavior->PushSucessorState( new Idle );
}
