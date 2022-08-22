#include "BvPlayerXDash.h"
#include "BvPlayerXIdle.h"

void PlayerX::Dash::Activate( PlayerX& playerX, Scene& scene )
{
	switch ( playerX.attackState )
	{
	case AttackState::NoAttack:
	case AttackState::Charge:
		{
			playerX.SetAnimation( PlayerX::AnimationState::DashStart, animStartSpeed );
		}
		break;
	case AttackState::Shoot:
	case AttackState::ShootMid:
	case AttackState::ShootMax:
		{
			playerX.SetAnimation( PlayerX::AnimationState::ShootDashStart, animStartSpeed );
		}
		break;
	}
}

PlayerX::Behavior* PlayerX::Dash::Update( PlayerX& playerX, Scene& scene, float dt )
{
	if ( HasSucessors() )
	{
		playerX.SetDashEnd();
		//if ( IsDashEnd( playerX, scene, dt ) )
		{
			return PassTorch();
		}
	}

	switch ( playerX.attackState )
	{
	case AttackState::NoAttack:
	case AttackState::Charge:
		{
			if ( playerX.curAnimState == AnimationState::DashStart && playerX.curAnimation.IsEnd() )
			{
				playerX.SetAnimation( AnimationState::DashLoop, animLoopSpeed );
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

	DoDash( playerX, scene, dt );

	return nullptr;
}

void PlayerX::Dash::DoDash( PlayerX& playerX, Scene& scene, float dt )
{
	dashTime += dt;

	if ( dashTime <= dashMaxTime )
	{
		if ( playerX.isFacingRight )
		{
			playerX.vel.x = playerX.dashSpeed;
		}
		else
		{
			playerX.vel.x = -playerX.dashSpeed;
		}
		playerX.Move( dt, scene );
	}
	else
	{
		playerX.SetDashEnd();
	}
}

bool PlayerX::Dash::IsDashEnd( PlayerX& playerX, Scene& scene, float dt )
{
	if ( !isDashEndStart )
	{
		playerX.SetAnimation( PlayerX::AnimationState::DashEnd, animEndSpeed );
		isDashEndStart = true;
	}

	if ( playerX.curAnimation.IsEnd() )
	{
		playerX.SetDashEnd();
		return true;
	}
	return false;
}

void PlayerX::Dash::ChangeToShootAnim( PlayerX& playerX, Scene& scene, PlayerXBullet::Type type )
{
	if ( playerX.curAnimState == AnimationState::DashLoop )
	{
		playerX.SetAnimation( PlayerX::AnimationState::ShootDashLoop, animLoopSpeed );
	}
	playerX.SpawnBullet( type, scene, { bulletSpawnDashX, bulletSpawnDashY } );
}



void PlayerX::AirDash::Activate( PlayerX& playerX, Scene& scene )
{
	playerX.SetAnimation( PlayerX::AnimationState::AirDashStart, animStartSpeed );
}

PlayerX::AirDash::Behavior* PlayerX::AirDash::Update( PlayerX& playerX, Scene& scene, float dt )
{
	if ( HasSucessors() )
	{
		playerX.vel.x = 0.0f;
		playerX.canAirDash = false;
		return PassTorch();
	}

	if ( playerX.curAnimation.IsEnd() )
	{
		playerX.SetAnimation( PlayerX::AnimationState::DashLoop, animStartSpeed );
	}

	playerX.vel.y = 0.0f;
	DoDash( playerX, scene, dt );

	return nullptr;
}
