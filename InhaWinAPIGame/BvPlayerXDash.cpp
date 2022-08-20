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
		playerX.SetDashEnd();
		return PassTorch();
	}
	if ( playerX.curAnimState == AnimationState::DashStart && playerX.curAnimation.IsEnd() )
	{
		playerX.SetAnimation( AnimationState::DashLoop, animLoopSpeed );
	}

	DoDash( playerX, scene, dt );

	return nullptr;
}

void PlayerX::Dash::DoDash( PlayerX& playerX, Scene& scene, float dt )
{
	dashTime += dt;

	if ( dashTime <= dashMaxTime )
	{
		if ( playerX.isFacingRight )
		{
			playerX.vel.x = playerX.dashSpeed;
		}
		else
		{
			playerX.vel.x = -playerX.dashSpeed;
		}
		playerX.Move( dt, scene );
	}
	else
	{
		playerX.SetDashEnd();
		//playerX.pBehavior->PushSucessorState( new DashEnd );
	}
}

void DashEnd::Activate( PlayerX& playerX, Scene& scene )
{
	playerX.SetAnimation( PlayerX::AnimationState::DashEnd, animEndSpeed );
}

PlayerX::Behavior* DashEnd::Update( PlayerX& playerX, Scene& scene, float dt )
{
	if ( playerX.GetAnimation().IsEnd() )
	{
		playerX.SetDashEnd();
	}
	return nullptr;
}
