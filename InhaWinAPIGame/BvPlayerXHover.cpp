#include "BvPlayerXHover.h"


void PlayerX::Hover::Activate( PlayerX& playerX, Scene& scene )
{
	playerX.SetAnimation( PlayerX::AnimationState::Hover, 0.0f );
	playerX.vel = { 0.0f,0.0f };
}

PlayerX::Behavior* PlayerX::Hover::Update( PlayerX& playerX, Scene& scene, float dt )
{
	if ( HasSucessors() )
	{
		return PassTorch();
	}

	hoverTime += dt;
	if ( hoverTime <= hoverMaxTime )
	{
		HoverEffect( playerX, scene, dt );

		if ( IsInputOthers( playerX ) )
		{
			playerX.hoverCount++;
		}
	}
	else
	{
		playerX.hoverCount++;
	}
	
	return nullptr;
}

bool PlayerX::Hover::IsInputOthers( PlayerX& playerX )
{
	return playerX.isZKeyDown || playerX.isRightKeyDown || playerX.isLeftKeyDown;
}

void PlayerX::Hover::HoverEffect( PlayerX& playerX, Scene& scene, float dt )
{
	hoverEffectTime += dt;
	if ( hoverEffectTime >= hoverEffectMaxTime )
	{
		if ( hoverEffectCount % 2 == 0 )
		{
			playerX.vel.y = hoverEffectMaxSpeed;
		}
		else
		{
			playerX.vel.y = -hoverEffectMaxSpeed;
		}
		playerX.Move( dt, scene );
		hoverEffectCount++;
		hoverEffectTime = 0.0f;
	}
}
