#include "BvPlayerXShoot.h"

void PlayerX::Shoot::Activate( PlayerX& playerX, Scene& scene )
{
	playerX.SetAnimation( PlayerX::AnimationState::Shoot, animSpeed );
	playerX.SpawnBullet1( scene );
}

PlayerX::Behavior* PlayerX::Shoot::Update( PlayerX& playerX, Scene& scene, float dt )
{
	if ( playerX.curAnimation.IsEnd() )
	{
		playerX.attackState = AttackState::NoAttack;
		if ( HasSucessors() )
		{
			return PassTorch();
		}
	}
	return nullptr;
}
