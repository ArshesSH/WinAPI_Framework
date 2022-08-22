#include "BvSigmaHeadOpen.h"

#include "BvSigmaHeadDamaged.h"
#include "Scene.h"

void SigmaHead::Open::Activate( SigmaHead& sigma, Scene& scene )
{
	sigma.SetAnimation( AnimationState::Open, animSpeed );
}

SigmaHead::Behavior* SigmaHead::Open::Update( SigmaHead& sigma, Scene& scene, float dt )
{
	if ( sigma.curAnimation.IsEnd() )
	{
		sigma.curAnimation.SetStop();
		sigma.SetInvincible( false );
	}

	if ( sigma.isDamaged )
	{
		if ( HasSucessors() )
		{
			sigma.isDamaged = false;
			sigma.SetInvincible( true );
			
			sigma.pBehavior->PushSucessorState( new Damaged );
			return PassTorch();
		}
	}


	time += dt;

	if ( time >= openTime )
	{
		if ( HasSucessors() )
		{
			sigma.SetInvincible( true );
			return PassTorch();
		}
	}

	return nullptr;
}
