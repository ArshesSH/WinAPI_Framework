#include "BvSigmaHeadFade.h"

#include "Scene.h"
#include "BvSigmaHeadDecide.h"

void SigmaHead::Fade::Activate( SigmaHead& sigma, Scene& scene )
{
	sigma.SetAnimation( AnimationState::Fade, animSpeed );
}

SigmaHead::Behavior* SigmaHead::Fade::Update( SigmaHead& sigma, Scene& scene, float dt )
{
	time += dt;

	if ( !isFadeIn )
	{
		if ( time >= playTime )
		{
			sigma.SetPos( resetPos );
			time = 0.0f;
			isFadeIn = true;
		}
	}
	else
	{
		if ( time >= playTime )
		{
			sigma.pBehavior->PushSucessorState( new Decide( scene.GetRandomEngine() ) );
			return PassTorch();
		}
	}

	return nullptr;
}
