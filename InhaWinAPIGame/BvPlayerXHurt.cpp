#include "BvPlayerXHurt.h"

#include "BvPlayerXIdle.h"
#include "BvPlayerXAirbone.h"

void PlayerX::Hurt::Activate( PlayerX& playerX, Scene& scene )
{
	playerX.SetAnimation( PlayerX::AnimationState::HurtA, animStartSpeed );
}

PlayerX::Behavior* PlayerX::Hurt::Update( PlayerX& playerX, Scene& scene, float dt )
{
	if ( playerX.curAnimation.IsEnd() )
	{
		playerX.curAnimation.SetStop();
	}

	time += dt;
	if ( time >= hurtResetTime )
	{
		if ( playerX.isOnGround )
		{
			playerX.pBehavior->PushSucessorState( new Idle );
		}
		else
		{
			playerX.pBehavior->PushSucessorState( new Airbone );
		}

		if ( HasSucessors() )
		{
			return PassTorch();
		}
	}
	return nullptr;
}
