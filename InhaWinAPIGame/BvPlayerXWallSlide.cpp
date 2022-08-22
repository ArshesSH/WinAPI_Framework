#include "BvPlayerXWallSlide.h"

void PlayerX::WallSlide::Activate( PlayerX& playerX, Scene& scene )
{
	switch ( playerX.attackState )
	{
	case AttackState::NoAttack:
	case AttackState::Charge:
		{
			playerX.SetAnimation( PlayerX::AnimationState::WallCling, animStartSpeed );
		}
		break;
	case AttackState::Shoot:
	case AttackState::ShootMid:
	case AttackState::ShootMax:
		{
			playerX.SetAnimation( PlayerX::AnimationState::ShootWallCling, animStartSpeed );
		}
		break;
	}

	playerX.vel = { 0.0f, wallSlideSpeed };
	playerX.SetStopFacingTrack();
	playerX.hoverCount = 0;
}

PlayerX::Behavior* PlayerX::WallSlide::Update( PlayerX& playerX, Scene& scene, float dt )
{
	if ( HasSucessors() )
	{
		playerX.SetStopFacingTrack(false);
		playerX.vel.y = 0.0f;
		return PassTorch();
	}

	if ( !(GetAsyncKeyState( 'X' ) & 0x8001) )
	{
		checkReinput = true;
	}
	if ( checkReinput )
	{
		playerX.vel.x = 0.0f;
		if ( playerX.isXKeyDown )
		{
			playerX.moveState = MoveState::WallKick;
			playerX.UpdatePlayerBehavior();
			return PassTorch();
		}
	}


	switch ( playerX.attackState )
	{
	case AttackState::NoAttack:
	case AttackState::Charge:
		{
			if ( playerX.curAnimState == AnimationState::WallCling && playerX.curAnimation.IsEnd() )
			{
				playerX.SetAnimation( AnimationState::WallSlide, 0.0f );
			}
		}
		break;
	case AttackState::Shoot:
		{
			ChangeToShootAnim( playerX, scene, PlayerXBullet::Type::Bullet1, PlayerX::AnimationState::ShootWallSlide );
		}
		break;
	case AttackState::ShootMid:
		{
			ChangeToShootAnim( playerX, scene, PlayerXBullet::Type::Bullet2, PlayerX::AnimationState::ShootWallSlide );
		}
		break;

	case AttackState::ShootMax:
		{
			ChangeToShootAnim( playerX, scene, PlayerXBullet::Type::Bullet3, PlayerX::AnimationState::ShootWallSlide );
		}
		break;
	}


	if (( playerX.isFacingRight && playerX.isLeftKeyDown )|| (!playerX.isFacingRight && playerX.isRightKeyDown ))
	{
		playerX.moveState = MoveState::Airbone;
		playerX.UpdatePlayerBehavior();
	}
	playerX.Move( dt, scene );

	return nullptr;
}

void PlayerX::WallSlide::ChangeToShootAnim( PlayerX& playerX, Scene& scene, PlayerXBullet::Type type, PlayerX::AnimationState targetAnim )
{
	if ( playerX.curAnimState == AnimationState::WallSlide )
	{
		int idx = playerX.curAnimation.GetFrameIndex();
		playerX.SetAnimation( targetAnim, animSpeed );
		playerX.curAnimation.SetFrameIndex( idx );
		isStartResetAnimation = true;
	}
	playerX.SpawnBullet( type, scene, { bulletSpawnDefaultX, bulletSpawnDefaultY }, true );
	time = 0.0f;
}
