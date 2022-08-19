#pragma once

#include "PlayerXBehavior.h"

class PlayerXWalk : public PlayerXBehavior
{
public:
	PlayerXWalk( PlayerX& playerX )
		:
		PlayerXBehavior( playerX )
	{}
	void Do( float dt, Scene& scene ) override
	{
	}

private:

private:
	static constexpr float walkLoopSpeed = 0.1f;
	static constexpr float walkStartSpeed = 0.2f;
};