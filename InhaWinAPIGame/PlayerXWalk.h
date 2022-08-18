#pragma once

#include "PlayerXBehavior.h"

class PlayerXWalk : public PlayerXBehavior
{
public:
	PlayerXWalk( PlayerX& playerX )
		:
		PlayerXBehavior( playerX, PlayerX::State::WalkStart )
	{}
	void Do( float dt, Scene& scene ) override
	{
		const auto curXState = playerX.GetState();
		const auto curXAnim = playerX.GetAnimation();

		if ( curXState == PlayerX::State::WalkStart )
		{
			if ( curXAnim.IsEnd() )
			{
				playerX.SetState( PlayerX::State::WalkLoop );
				playerX.SetAnimation( PlayerX::State::WalkLoop, walkLoopSpeed );
			}
		}
		else if ( curXState == PlayerX::State::WalkLoop )
		{

		}
		else
		{
			playerX.SetState( PlayerX::State::WalkStart );
			playerX.SetAnimation( PlayerX::State::WalkStart, walkStartSpeed );
		}
	}

private:

private:
	static constexpr float walkLoopSpeed = 0.3f;
	static constexpr float walkStartSpeed = 0.2f;
};