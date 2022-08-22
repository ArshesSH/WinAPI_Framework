#include "BvSigmaHeadFade.h"

void SigmaHead::Fade::Activate( SigmaHead& sigma, Scene& scene )
{
	sigma.SetAnimation( AnimationState::Fade, animSpeed );
}

SigmaHead::Behavior* SigmaHead::Fade::Update( SigmaHead& sigma, Scene& scene, float dt )
{
	time += dt;

	if ( isFadeIn )
	{
		if ( time >= playTime )
		{
			sigma.SetPos( resetPos );
			time = 0.0f;
		}
	}
	else
	{
		if ( time >= playTime )
		{
			return PassTorch();
		}
	}

	return nullptr;
}
