#pragma once

#include "Scene.h"
#include "PlayerX.h"

class PlayerXBehavior
{
public:
	PlayerXBehavior( class PlayerX& playerX, PlayerX::State state )
		:
		playerX( playerX ),
		behaviorState( state )
	{}
	virtual ~PlayerXBehavior() = default;

	virtual void Do(float dt, class Scene& scene ) = 0;

	bool IsStateSameWith( PlayerX::State state ) const
	{
		return behaviorState == state;
	}

protected:
	PlayerX& playerX;
	PlayerX::State behaviorState;
};

