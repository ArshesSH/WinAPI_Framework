#include "BvSigmaHeadElectricAttack.h"

#include "Scene.h"
#include "SigmaElectricBall.h"

void SigmaHead::ElectricAttack::Activate( SigmaHead& sigma, Scene& scene )
{
	scene.AccessBulletPtrs().push_back( std::make_unique<SigmaElectricBall>(
		sigma.GetPos() + Vec2<float>{ ballRelPosX1, ballRelPosY1 },
		attackSpeed,
		Vec2<float>{ 0.0f, 0.0f }
	) );
	scene.AccessBulletPtrs().push_back( std::make_unique<SigmaElectricBall>(
		sigma.GetPos() + Vec2<float>{ ballRelPosX2, ballRelPosY2 },
		attackSpeed,
		Vec2<float>{ 0.0f, 0.0f }
	) );
}

SigmaHead::Behavior* SigmaHead::ElectricAttack::Update( SigmaHead& sigma, Scene& scene, float dt )
{
	time += dt;

	if ( sigma.curAnimation.IsEnd() )
	{
		sigma.curAnimation.SetStop();
	}

	if ( time >= SigmaElectricBall::lifeTime )
	{
		if ( GetLengthSqFromPlayer( sigma, scene ) >= chaseEndDistance )
		{
			sigma.vel = GetDirToPlayer( sigma, scene ) * sigma.moveSpeed;
			sigma.Move( dt, scene );
		}
		else
		{
			if ( HasSucessors() )
			{
				return PassTorch();
			}
		}
	}

	return nullptr;
}

Vec2<float> SigmaHead::ElectricAttack::GetDirToPlayer( SigmaHead& sigma, Scene& scene ) const
{
	const auto& pPlayer = scene.FindPlayer();
	const auto playerPos = pPlayer->GetPos() + Vec2<float>{0, 100.0f};
	const auto sigmaPos = sigma.GetPos();

	return (playerPos - sigmaPos).GetNormalized();
}

float SigmaHead::ElectricAttack::GetLengthSqFromPlayer( SigmaHead& sigma, Scene& scene ) const
{
	const auto& pPlayer = scene.FindPlayer();
	const auto playerPos = pPlayer->GetPos();
	const auto sigmaPos = sigma.GetPos();

	return (playerPos - sigmaPos).GetLengthSq();
}
