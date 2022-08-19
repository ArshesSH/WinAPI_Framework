#include "BvPlayerXDash.h"

#ifndef NDEBUG
#include <iostream>
#endif // !NDEBUG


void PlayerX::Dash::Activate( PlayerX& playerX, Scene& scene )
{
	playerX.SetAnimation( PlayerX::AnimationState::DashStart, dashStartSpeed );
}

PlayerX::Behavior* PlayerX::Dash::Update( PlayerX& playerX, Scene& scene, float dt )
{
	if ( HasSucessors() )
	{
		return PassTorch();
	}

	if ( playerX.GetAnimationState() == AnimationState::DashStart &&  playerX.curAnimation.IsEnd() )
	{
		playerX.SetAnimation( AnimationState::DashLoop, dashLoopSpeed );
	}

	if ( playerX.GetState() != State::Walk )
	{
		playerX.SetMoveSpeed( dashMoveSpeed );
	}
	

	dashTime += dt;
	if ( dashTime <= dashMaxTime && playerX.IsDash() )
	{
		if ( playerX.IsFacingRight() )
		{
			if ( playerX.isLeftKeyDown )
			{
				playerX.StopDash();
			}

			Vec2<float> vel = { playerX.GetVel().x + dashMoveSpeed, 0 };
			if ( !playerX.isRightKeyDown )
			{
				vel.x += 200.0f;
			}
			playerX.SetVel( vel );
		}
		else
		{
			if ( playerX.isRightKeyDown )
			{
				playerX.StopDash();
			}

			Vec2<float> vel = { playerX.GetVel().x - dashMoveSpeed, 0 };
			if ( !playerX.isLeftKeyDown )
			{
				vel.x -= 200.0f;
			}
			playerX.SetVel( vel );
		}
		
		playerX.Move( dt, scene );
	}
	else
	{
		playerX.StopDash();
	}
	
#ifndef NDBUG
	std::cout << "DashTime = " << dashTime << std::endl;
#endif // !NDBUG


	return nullptr;
}
