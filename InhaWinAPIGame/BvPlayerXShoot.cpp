#include "BvPlayerXShoot.h"

void PlayerX::Shoot::Activate( PlayerX& playerX, Scene& scene )
{
	playerX.SetAnimation( PlayerX::AnimationState::Shoot, animSpeed );
	playerX.SpawnBullet1( scene );
}

PlayerX::Behavior* PlayerX::Shoot::Update( PlayerX& playerX, Scene& scene, float dt )
{
	if ( HasSucessors() )
	{
		playerX.pBehavior->PushSucessorState( new ShootEnd );
		return PassTorch();
	}

	if ( playerX.curAnimation.IsEnd() )
	{

	}
	return nullptr;
}

void ShootEnd::Activate( PlayerX& playerX, Scene& scene )
{
	playerX.SetAnimation( PlayerX::AnimationState::ShootEnd, animSpeed );
}

PlayerX::Behavior* ShootEnd::Update( PlayerX& playerX, Scene& scene, float dt )
{
	if ( playerX.GetAnimation().IsEnd() )
	{
		return PassTorch();
	}
	return nullptr;
}
