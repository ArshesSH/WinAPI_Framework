#include "BvPlayerXWalk.h"
#include "Scene.h"

void PlayerX::Walk::Activate( PlayerX& playerX, Scene& scene )
{
	switch ( playerX.attackState )
	{
	case AttackState::NoAttack:
	case AttackState::Charge:
		{
			playerX.SetAnimation( PlayerX::AnimationState::WalkStart, animStartSpeed );
			oldAnimState = AnimationState::WalkStart;
		}
		break;
	case AttackState::Shoot:
	case AttackState::ShootMid:
	case AttackState::ShootMax:
		{
			playerX.SetAnimation( PlayerX::AnimationState::ShootWalkStart, animStartSpeed );
			oldAnimState = AnimationState::ShootWalkStart;
		}
		break;
	}

}

PlayerX::Behavior* PlayerX::Walk::Update( PlayerX& playerX, Scene& scene, float dt )
{
	if ( HasSucessors() )
	{
		playerX.vel.x = 0.0f;
		return PassTorch();
	}

	switch ( playerX.attackState )
	{
	case AttackState::NoAttack:
	case AttackState::Charge:
		{
			if ( playerX.curAnimState == AnimationState::WalkStart && playerX.curAnimation.IsEnd() )
			{
				playerX.SetAnimation( AnimationState::WalkLoop, animLoopSpeed );
			}
		}
		break;
	case AttackState::Shoot:
		{
			ChangeToShootAnim( playerX, scene, PlayerXBullet::Type::Bullet1 );
		}
		break;
	case AttackState::ShootMid:
		{
			ChangeToShootAnim( playerX, scene, PlayerXBullet::Type::Bullet2 );
		}
		break;

	case AttackState::ShootMax:
		{
			ChangeToShootAnim( playerX, scene, PlayerXBullet::Type::Bullet3 );
		}
		break;
	}

	if ( isStartResetAnimation )
	{
		time += dt;
		if ( time >= walkResetTime )
		{
			if ( playerX.curAnimation.IsEnd() )
			{
				int idx = playerX.curAnimation.GetFrameIndex();
				playerX.SetAnimation( AnimationState::WalkLoop, animLoopSpeed );
				playerX.curAnimation.SetFrameIndex( idx );
				isStartResetAnimation = false;
				time = 0.0f;
			}
		}
	}

	DoWalk( playerX, scene, dt );

	return nullptr;
}

void PlayerX::Walk::DoWalk( PlayerX & playerX, Scene & scene, float dt )
{
	if ( playerX.isRightKeyDown )
	{
		playerX.vel.x = playerX.defaultMoveSpeed;
	}
	if ( playerX.isLeftKeyDown )
	{
		playerX.vel.x = -playerX.defaultMoveSpeed;
	}
	playerX.Move( dt, scene );

#ifndef NDBUG
	//std::cout << "PlayerSpeedX = " << playerX.vel.x << std::endl;
#endif // !NDBUG

}

void PlayerX::Walk::ChangeToShootAnim( PlayerX& playerX, Scene& scene, PlayerXBullet::Type type )
{

	if ( playerX.curAnimState == AnimationState::WalkLoop )
	{
		int idx = playerX.curAnimation.GetFrameIndex();
		playerX.SetAnimation( PlayerX::AnimationState::ShootWalkLoop, animLoopSpeed );
		playerX.curAnimation.SetFrameIndex( idx );
		isStartResetAnimation = true;
	}
	playerX.SpawnBullet( type, scene );
	time = 0.0f;
}
