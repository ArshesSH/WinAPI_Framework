#include "BvSigmaHeadReady.h"

#include "Scene.h"

void SigmaHead::Ready::Activate( SigmaHead& sigma, Scene& scene )
{
	sigma.SetAnimation( AnimationState::Ready, animSpeed );
}

SigmaHead::Behavior* SigmaHead::Ready::Update( SigmaHead& sigma, Scene& scene, float dt )
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
