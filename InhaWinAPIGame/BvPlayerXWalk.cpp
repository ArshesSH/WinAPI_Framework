#include "BvPlayerXWalk.h"
#include "Scene.h"

void PlayerX::Walk::Activate( PlayerX& playerX, Scene& scene )
{
	if ( playerX.attackState == AttackState::NoAttack )
	{
		playerX.SetAnimation( PlayerX::AnimationState::WalkStart, animStartSpeed );
		oldAnimState = AnimationState::WalkStart;
	}
	else
	{
		playerX.SetAnimation( PlayerX::AnimationState::ShootWalkStart, animStartSpeed );
		oldAnimState = AnimationState::ShootWalkStart;
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
			playerX.SetAnimation( AnimationState::WalkLoop, animLoopSpeed );
			isStartResetAnimation = false;
			time = 0.0f;
		}
	}

    DoWalk( playerX, scene, dt );

    return nullptr;
}

void PlayerX::Walk::DoWalk( PlayerX& playerX, Scene& scene, float dt )
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



/*

	if ( playerX.curAnimState == AnimationState::WalkLoop )
	{
		const int idx = playerX.curAnimation.GetFrameIndex();
		playerX.SetAnimation( PlayerX::AnimationState::ShootWalkLoop, animLoopSpeed );
		playerX.curAnimation.SetFrameIndex( idx );
		playerX.SpawnBullet( type, scene );
		isStartResetAnimation = true;
	}

*/