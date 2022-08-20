#include "BvPlayerXHover.h"

void PlayerX::Hover::Activate( PlayerX& playerX, Scene& scene )
{
	playerX.SetAnimation( PlayerX::AnimationState::HoverFront, 0.0f );
}

PlayerX::Behavior* PlayerX::Hover::Update( PlayerX& playerX, Scene& scene, float dt )
{
	if ( HasSucessors() )
	{
		return PassTorch();
	}

	playerX.vel.y = 0.0f;
	playerX.Move( dt, scene );


	return nullptr;
}
