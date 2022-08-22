#include "BvPlayerXCrouch.h"

void PlayerX::Crouch::Activate( PlayerX& playerX, Scene& scene )
{


	switch ( playerX.attackState )
	{
	case AttackState::NoAttack:
	case AttackState::Charge:
		{
			playerX.SetAnimation( PlayerX::AnimationState::Crouch, animSpeed );
			playerX.vel = { 0.0f, 0.0f };
		}
		break;
	case AttackState::Shoot:
	case AttackState::ShootMid:
	case AttackState::ShootMax:
		{
			playerX.SetAnimation( PlayerX::AnimationState::CrouchShootStart, animSpeed );
			playerX.vel = { 0.0f, 0.0f };
		}
		break;
	}

}

PlayerX::Behavior* PlayerX::Crouch::Update( PlayerX& playerX, Scene& scene, float dt )
{
	if ( HasSucessors() )
	{
		return PassTorch();
	}

	switch ( playerX.attackState )
	{
	case AttackState::Shoot:
		{
			if ( playerX.curAnimState == AnimationState::Crouch )
			{
				playerX.SetAnimation( PlayerX::AnimationState::CrouchShoot, animSpeed );
				isStartResetAnimation = true;
			}
			playerX.SpawnBullet( PlayerXBullet::Type::Bullet1, scene, { bulletSpawnDashX, bulletSpawnDashY } );
			time = 0.0f;
		}
		break;
	case AttackState::ShootMid:
		{
			if ( playerX.curAnimState == AnimationState::Crouch )
			{
				playerX.SetAnimation( PlayerX::AnimationState::CrouchShoot, animSpeed );
				isStartResetAnimation = true;
			}
			playerX.SpawnBullet( PlayerXBullet::Type::Bullet2, scene, { bulletSpawnDashX, bulletSpawnDashY } );
			time = 0.0f;
		}
		break;
	case AttackState::ShootMax:
		{
			if ( playerX.curAnimState == AnimationState::Crouch )
			{
				playerX.SetAnimation( PlayerX::AnimationState::CrouchShootCharged, animSpeed );
				isStartResetAnimation = true;
			}
			playerX.SpawnBullet( PlayerXBullet::Type::Bullet3, scene, { bulletSpawnDashX, bulletSpawnDashY } );
			time = 0.0f;
		}
		break;
	}

	if ( isStartResetAnimation )
	{
		time += dt;
		if ( time >= animResetTime )
		{
			if ( playerX.curAnimation.IsEnd() )
			{
				playerX.SetAnimation( AnimationState::Crouch, animSpeed );
				playerX.curAnimation.SetFrameIndex( 1 );
				isStartResetAnimation = false;
				time = 0.0f;
			}
		}
	}


	int dirChange = 0;

	if ( playerX.isLeftKeyDown )
	{
		dirChange--;
	}
	if ( playerX.isRightKeyDown )
	{
		dirChange++;
	}

	if ( dirChange > 0 )
	{
		playerX.isFacingRight = true;
	}
	else if ( dirChange < 0 )
	{
		playerX.isFacingRight = false;
	}


	if ( playerX.curAnimState == AnimationState::Crouch && playerX.curAnimation.IsEnd() )
	{
		playerX.curAnimation.SetStop();
	}


	return nullptr;
}
