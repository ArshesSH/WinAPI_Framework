#include "BvPlayerXIdle.h"

#include "Scene.h"

void PlayerX::Idle::Activate( PlayerX& playerX, Scene& scene )
{
	playerX.SetAnimation( PlayerX::AnimationState::Idle, idleSpeed );
}

PlayerX::Behavior* PlayerX::Idle::Update( PlayerX& playerX, Scene& scene, float dt )
{
	if ( HasSucessors() )
	{
		return PassTorch();
	}

	switch ( playerX.attackState )
	{
	case AttackState::NoAttack:
		{
			SetRandomAnimation( playerX );
		}
		break;
	case AttackState::Shoot:
		{
			playerX.SetAnimation( PlayerX::AnimationState::Shoot, shootSpeed );
			playerX.SpawnBullet( PlayerXBullet::Type::Bullet1, scene, {bulletSpawnDefaultX, bulletSpawnDefaultY} );
		}
		break;
	case AttackState::ShootMid:
		{
			playerX.SetAnimation( PlayerX::AnimationState::Shoot, shootSpeed );
			playerX.SpawnBullet( PlayerXBullet::Type::Bullet2, scene, { bulletSpawnDefaultX, bulletSpawnDefaultY } );
		}
		break;
	case AttackState::ShootMax:
		{
			playerX.SetAnimation( PlayerX::AnimationState::Shoot, shootSpeed );
			playerX.SpawnBullet( PlayerXBullet::Type::Bullet3, scene, { bulletSpawnDefaultX, bulletSpawnDefaultY } );
		}
		break;
	}

	return nullptr;
}

void PlayerX::Idle::SetRandomAnimation( PlayerX& playerX )
{
	const auto curAnim = playerX.GetAnimation();

	if ( curAnim.IsEnd() )
	{
		if ( playerX.GetAnimationState() == PlayerX::AnimationState::Idle )
		{
			idlePlayCnt++;
		}

		if ( idlePlayCnt == idleMaxCount )
		{
			playerX.SetAnimation( PlayerX::AnimationState::IdleBlink, idleBlinkSpeed );
			idlePlayCnt = 0;
		}
		else
		{
			playerX.SetAnimation( PlayerX::AnimationState::Idle, idleSpeed );
		}
	}
}