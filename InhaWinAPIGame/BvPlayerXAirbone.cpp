#include "BvPlayerXAirbone.h"

void PlayerX::Airbone::Activate( PlayerX& playerX, Scene& scene )
{
	switch ( playerX.attackState )
	{
	case AttackState::NoAttack:
	case AttackState::Charge:
		{
			playerX.SetAnimation( PlayerX::AnimationState::Airbone, 0.0f );
		}
		break;
	case AttackState::Shoot:
	case AttackState::ShootMid:
	case AttackState::ShootMax:
		{
			playerX.SetAnimation( PlayerX::AnimationState::ShootAirbone, 0.0f );
		}
		break;
	}
}

PlayerX::Behavior* PlayerX::Airbone::Update( PlayerX& playerX, Scene& scene, float dt )
{
	if ( HasSucessors() )
	{
		playerX.vel.y = 0.0f;
		return PassTorch();
	}

	switch ( playerX.attackState )
	{
	case AttackState::Shoot:
		{
			ChangeToShootAnim( playerX, scene, PlayerXBullet::Type::Bullet1, PlayerX::AnimationState::ShootAirbone );
		}
		break;
	case AttackState::ShootMid:
		{
			ChangeToShootAnim( playerX, scene, PlayerXBullet::Type::Bullet2, PlayerX::AnimationState::ShootAirbone );
		}
		break;

	case AttackState::ShootMax:
		{
			ChangeToShootAnim( playerX, scene, PlayerXBullet::Type::Bullet3, PlayerX::AnimationState::ShootAirbone );
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
				int idx = playerX.curAnimation.GetFrameIndex();
				playerX.SetAnimation( AnimationState::Jump, jumpSpeed );
				playerX.curAnimation.SetFrameIndex( idx );
				isStartResetAnimation = false;
				time = 0.0f;
			}
		}
	}


	playerX.vel.y = playerX.gravity.GetGravityVel( playerX.vel, dt );
	if ( playerX.isZKeyDown )
	{
		if ( playerX.isRightKeyDown )
		{
			playerX.vel.x = playerX.dashSpeed;
		}
		if ( playerX.isLeftKeyDown )
		{
			playerX.vel.x = -playerX.dashSpeed;
		}
	}
	else
	{
		if ( playerX.isRightKeyDown )
		{
			playerX.vel.x = playerX.defaultMoveSpeed;
		}
		if ( playerX.isLeftKeyDown )
		{
			playerX.vel.x = -playerX.defaultMoveSpeed;
		}
	}

	playerX.Move( dt, scene );

	return nullptr;
}

void PlayerX::Airbone::ChangeToShootAnim( PlayerX& playerX, Scene& scene, PlayerXBullet::Type type, PlayerX::AnimationState targetAnim )
{
	if ( playerX.curAnimState == AnimationState::Airbone )
	{
		int idx = playerX.curAnimation.GetFrameIndex();
		playerX.SetAnimation( targetAnim, animSpeed );
		playerX.curAnimation.SetFrameIndex( idx );
		isStartResetAnimation = true;
	}
	playerX.SpawnBullet( type, scene, { bulletSpawnDefaultX, bulletSpawnDefaultY } );
	time = 0.0f;
}
