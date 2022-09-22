#include "BvSigmaHeadDamaged.h"

void SigmaHead::Damaged::Activate( SigmaHead& sigma, Scene& scene )
{
	sigma.SetAnimation( AnimationState::Damaged, animSpeed );
}

SigmaHead::Behavior* SigmaHead::Damaged::Update( SigmaHead& sigma, Scene& scene, float dt )
{
	time += dt;

	if ( time >= playTime )
	{
		if ( HasSucessors() )
		{
			return PassTorch();
		}
	}

	return nullptr;
}
