#include "BvPlayerXDash.h"
#include "BvPlayerXIdle.h"

#ifndef NDEBUG
#include <iostream>
#endif // !NDEBUG


void PlayerX::Dash::Activate( PlayerX& playerX, Scene& scene )
{
	playerX.SetAnimation( PlayerX::AnimationState::DashStart, dashStartSpeed );
	isLastDirRight = playerX.isFacingRight;
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
	
	if ( isLastDirRight != playerX.isFacingRight )
	{
		playerX.StopDash();
	}


	dashTime += dt;
	if ( dashTime <= dashMaxTime && playerX.IsDash() )
	{
		if ( playerX.IsFacingRight() )
		{
			Vec2<float> vel = { playerX.GetVel().x + dashMoveSpeed, 0 };
			if ( !playerX.isRightKeyDown )
			{
				vel.x += 200.0f;
			}
			playerX.SetVel( vel );
		}
		else
		{
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
	//std::cout << "DashTime = " << dashTime << std::endl;
	//std::cout << std::boolalpha << playerX.isLeftKeyDown << std::endl;
#endif // !NDBUG


	return nullptr;
}
