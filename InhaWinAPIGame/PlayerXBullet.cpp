#include "PlayerXBullet.h"

#include "Scene.h"

void PlayerXBullet::Update( float dt, Scene& scene )
{
	switch ( type )
	{
	case PlayerXBullet::Type::Bullet1:
		if ( animationState != Type::Bullet1 )
		{
			damage = 3;
			animationState = Type::Bullet1;
			curAnimation = animationMap[(int)Type::Bullet1];
			animSpeed = bullet1AnimSpeed;
		}
		break;
	case PlayerXBullet::Type::Bullet2:
		if ( animationState != Type::Bullet2 )
		{
			damage = 4;
			animationState = Type::Bullet2;
			curAnimation = animationMap[(int)Type::Bullet2];
			animSpeed = bullet2AnimSpeed;
			this->ChangeColliderSize( { bullet2Width, bullet2Height } );
		}
		break;
	case PlayerXBullet::Type::Bullet3:
		if ( animationState != Type::Bullet3 )
		{
			damage = 8;
			animationState = Type::Bullet3;
			curAnimation = animationMap[(int)Type::Bullet3];
			animSpeed = bullet3AnimSpeed;
			this->ChangeColliderSize( { bullet3Width, bullet3Height } );
		}
		break;
	}
	if ( curAnimation.IsEnd() )
	{
		curAnimation.SetStop();
	}

	curAnimation.Update( dt, animSpeed );

	// Base class Update
	time += dt;
	if ( time >= lifeCycle )
	{
		SetDestroy();
	}

	if ( !isPlayHit )
	{
		Move( dt, scene );

		const auto& enemies = scene.FindActorByTag( oppositeTag );
		for ( const auto& enemy : enemies )
		{
			if ( enemy->IsCollideWith( scene.GetCollisionManager(), *(this->GetColliderPtr()) ) )
			{
				enemy->ApplyDamage( damage );


				vel.x = 0.0f;
				if ( enemy->IsImune() )
				{
					ChangeAnimationToImmune();
				}
				else
				{
					ChangeAnimationToHit();
				}
			}
		}
	}

	if ( IsCollideWithWall( GetNextPos( dt ), scene ) || IsCollideWithGround( GetNextPos( dt ), scene ) )
	{
		if ( !isPlayHit )
		{
			ChangeAnimationToHit();
		}
	}

	if ( CheckHitAnimationFinish() )
	{
		SetDestroy();
	}

}

void PlayerXBullet::SetTransform( const Mat3<float>& transform )
{
	curAnimation.SetTransform( transform );
}

void PlayerXBullet::Draw( HDC hdc )
{
	if ( !isFacingRight )
	{
		curAnimation.PlayByCamGDI( hdc, spriteFlipped, Vec2<int>( GetPos() ), 2, chroma, true );
	}
	else
	{
		curAnimation.PlayByCamGDI( hdc, sprite, Vec2<int>( GetPos() ), 2, chroma );
	}
}

void PlayerXBullet::ChangeAnimationToHit()
{
	switch ( type )
	{
	case PlayerXBullet::Type::Bullet1:
		type = Type::HitNormal;
		break;
	case PlayerXBullet::Type::Bullet2:
		type = Type::HitNormal;
		break;
	case PlayerXBullet::Type::Bullet3:
		type = Type::HitCharged;
		break;
	}

	curAnimation = animationMap[(int)type];
	isPlayHit = true;
}

void PlayerXBullet::ChangeAnimationToImmune()
{
	type = Type::HitImuned;
	curAnimation = animationMap[(int)type];
	isPlayHit = true;
}

bool PlayerXBullet::CheckHitAnimationFinish() const
{
	return (isPlayHit && curAnimation.IsEnd());
}
