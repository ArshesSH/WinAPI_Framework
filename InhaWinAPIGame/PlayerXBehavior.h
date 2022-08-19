#pragma once

#include "Scene.h"
#include "PlayerX.h"

class PlayerXBehavior
{
public:
	PlayerXBehavior( PlayerX& playerX )
		:
		playerX( playerX )
	{}
	virtual ~PlayerXBehavior() = default;

	virtual void Do(float dt, Scene& scene ) = 0;
	//virtual void TransferFrom( PlayerX::State state_in ) = 0;

protected:
	PlayerX& playerX;
};

