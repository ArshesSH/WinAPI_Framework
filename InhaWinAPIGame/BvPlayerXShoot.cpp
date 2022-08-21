#include "BvPlayerXShoot.h"

void PlayerX::Shoot::Activate( PlayerX& playerX, Scene& scene )
{
	playerX.SetAnimation( PlayerX::AnimationState::Shoot, animSpeed );
}

PlayerX::Behavior* PlayerX::Shoot::Update( PlayerX& playerX, Scene& scene, float dt )
{
	if ( playerX.curAnimation.IsEnd() )
	{
		//if ( playerX.attackState != AttackState::Shoot )
		{
			return PassTorch();
		}
	}
	return nullptr;
}
