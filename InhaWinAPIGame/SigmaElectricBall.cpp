#include "SigmaElectricBall.h"

#include "Scene.h"

void SigmaElectricBall::Update( float dt, Scene& scene )
{
	curAnimation.Update( dt, animSpeed );

	time += dt;

	if ( time >= attackDelayTime )
	{
		if ( time >= lifeTime )
		{
			SetDestroy();
		}
		else
		{
			if ( !isDirSet )
			{
				SetVel( GetDirToPlayer( scene ) * moveSpeed );
				isDirSet = true;
			}
			Move( dt, scene );
		}
	}
}

void SigmaElectricBall::SetTransform( const Mat3<float>& transform )
{
	curAnimation.SetTransform( transform );
}

void SigmaElectricBall::Draw( HDC hdc )
{
	curAnimation.PlayByCamGDI( hdc, sprite, Vec2<int>( GetPos() ), 2, chroma );
}

Vec2<float> SigmaElectricBall::GetDirToPlayer( class Scene& scene ) const
{
	const auto& pPlayer = scene.FindPlayer();

	const auto playerPos = pPlayer->GetPos() + Vec2<float>{-50, 60};
	const auto ballPos = GetPos();

	return (playerPos - ballPos).GetNormalized();
}
