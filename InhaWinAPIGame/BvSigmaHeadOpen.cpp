#include "BvSigmaHeadOpen.h"
void SigmaHead::Open::Activate( SigmaHead& sigma, Scene& scene )
{
	sigma.SetAnimation( AnimationState::Open, animSpeed );
}

SigmaHead::Behavior* SigmaHead::Open::Update( SigmaHead& sigma, Scene& scene, float dt )
{
	if ( sigma.curAnimation.IsEnd() )
	{
		if ( HasSucessors() )
		{
			return PassTorch();
		}
	}
	return nullptr;
}
