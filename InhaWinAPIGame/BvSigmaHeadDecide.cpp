#include "BvSigmaHeadDecide.h"

#include "BvSigmaHeadElectricAttack.h"
#include "BvSigmaHeadFade.h"
#include "BvSigmaHeadOpen.h"
#include "BvSigmaHeadReady.h"

#include "Scene.h"
#include <cassert>

void SigmaHead::Decide::Activate( SigmaHead& sigma, Scene& scene )
{
	sigma.SetAnimation( AnimationState::Idle, 0.0f );
}

SigmaHead::Behavior* SigmaHead::Decide::Update( SigmaHead& sigma, Scene& scene, float dt )
{
	time += dt;

	if ( time >= decideTime )
	{
		AttackType decidedAttack;
		if ( sigma.hp <= decideChangeHP )
		{
			decidedAttack = (AttackType)lessHPRandomGen( rng );
		}
		else
		{
			decidedAttack = (AttackType)attackRandomGen( rng );
		}

		switch ( decidedAttack )
		{
		case SigmaHead::Decide::AttackType::ElectricBall:
			sigma.pBehavior->SetSuccessorStates( {
				new Fade,
				new Open,
				new ElectricAttack,
				new Ready
				} );
			break;
		case SigmaHead::Decide::AttackType::Ball:
			sigma.pBehavior->SetSuccessorStates( {
				new Fade,
				new Open,
				new ElectricAttack,
				new Ready
				} );
			break;
		case SigmaHead::Decide::AttackType::Layser:
			sigma.pBehavior->SetSuccessorStates( {
				new Fade,
				new Open,
				new ElectricAttack,
				new Ready
				} );
			break;
		default:
			assert( false );
			break;
		}
		return PassTorch();
	}
	return nullptr;
}
