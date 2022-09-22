#include "BvPlayerXHover.h"


void PlayerX::Hover::Activate( PlayerX& playerX, Scene& scene )
{
	playerX.SetAnimation( PlayerX::AnimationState::Hover, 0.0f );
	playerX.vel = { 0.0f,hoverEffectMaxSpeed };
	playerX.SetStopFacingTrack();
}

PlayerX::Behavior* PlayerX::Hover::Update( PlayerX& playerX, Scene& scene, float dt )
{
	if ( HasSucessors() )
	{
		playerX.hoverCount++;
		playerX.vel.x = 0.0f;
		playerX.vel.y = 0.0f;
		playerX.SetStopFacingTrack(false);
		return PassTorch();
	}

	hoverTime += dt;
	if ( hoverTime <= hoverMaxTime )
	{
		if ( !(GetAsyncKeyState( VK_RIGHT ) & 0x8001) && !(GetAsyncKeyState( VK_LEFT ) & 0x8001) )
		{
			checkReinput = true;
		}

		if ( checkReinput )
		{
			playerX.vel.x = 0.0f;
			if ( playerX.isRightKeyDown )
			{
				playerX.vel.x = playerX.defaultMoveSpeed;
				hoverTime += 3*dt;
			}
			if ( playerX.isLeftKeyDown )
			{
				playerX.vel.x = -playerX.defaultMoveSpeed;
				hoverTime += 3*dt;
			}
		}
		ChangeAnimation( playerX );
		HoverEffect( playerX, scene, dt );
		playerX.Move( dt, scene );

	}
	else
	{
		playerX.hoverCount++;
	}
	
	return nullptr;
}


bool PlayerX::Hover::IsOtherKeyInputed( PlayerX& playerX )
{
	return playerX.isRightKeyDown || playerX.isLeftKeyDown;
}

void PlayerX::Hover::HoverEffect( PlayerX& playerX, Scene& scene, float dt )
{
	hoverEffectTime += dt;
	if ( hoverEffectTime >= hoverEffectMaxTime )
	{
		if ( hoverEffectCount % 2 == 0 )
		{
			playerX.vel.y = -hoverEffectMaxSpeed;
		}
		else
		{
			playerX.vel.y = hoverEffectMaxSpeed;
		}
		hoverEffectCount++;
		hoverEffectTime = 0.0f;
	}
}

void PlayerX::Hover::ChangeAnimation( PlayerX& playerX )
{
	if ( playerX.isFacingRight && playerX.vel.x > 0.0f )
	{
		if ( playerX.curAnimState != AnimationState::HoverFront )
		{
			playerX.SetAnimation( PlayerX::AnimationState::HoverFront, animSpeed );
		}
		else if( playerX.curAnimation.IsEnd() )
		{
			playerX.curAnimation.SetStop();
		}
	}
	else if ( playerX.isFacingRight && playerX.vel.x < 0.0f )
	{
		if ( playerX.curAnimState != AnimationState::HoverBack )
		{
			playerX.SetAnimation( PlayerX::AnimationState::HoverBack, animSpeed );
		}
		else if ( playerX.curAnimation.IsEnd() )
		{
			playerX.curAnimation.SetStop();
		}
	}
	else if ( !playerX.isFacingRight && playerX.vel.x > 0.0f )
	{
		if ( playerX.curAnimState != AnimationState::HoverBack )
		{
			playerX.SetAnimation( PlayerX::AnimationState::HoverBack, animSpeed );
		}
		else if ( playerX.curAnimation.IsEnd() )
		{
			playerX.curAnimation.SetStop();
		}
	}
	else if ( !playerX.isFacingRight && playerX.vel.x < 0.0f )
	{
		if ( playerX.curAnimState != AnimationState::HoverFront )
		{
			playerX.SetAnimation( PlayerX::AnimationState::HoverFront, animSpeed );
		}
		else if ( playerX.curAnimation.IsEnd() )
		{
			playerX.curAnimation.SetStop();
		}
	}
	else
	{
		if ( playerX.curAnimState != AnimationState::Hover )
		{
			playerX.SetAnimation( PlayerX::AnimationState::Hover, 0.0f );
		}
	}

}
