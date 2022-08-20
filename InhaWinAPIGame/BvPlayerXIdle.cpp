#include "BvPlayerXIdle.h"

#include "Scene.h"

void PlayerX::Idle::Activate( PlayerX& playerX, Scene& scene )
{
	playerX.SetAnimation( PlayerX::AnimationState::Idle, idleSpeed );
}

PlayerX::Behavior* PlayerX::Idle::Update( PlayerX& playerX, Scene& scene, float dt )
{
	if ( HasSucessors() )
	{
		return PassTorch();
	}

	SetRandomAnimation( playerX );
	return nullptr;
}

void PlayerX::Idle::SetRandomAnimation( PlayerX& playerX )
{
	const auto curAnim = playerX.GetAnimation();

	if ( curAnim.IsEnd() )
	{
		if ( playerX.GetAnimationState() == PlayerX::AnimationState::Idle )
		{
			idlePlayCnt++;
		}

		if ( idlePlayCnt == idleMaxCount )
		{
			playerX.SetAnimation( PlayerX::AnimationState::IdleBlink, idleBlinkSpeed );
			idlePlayCnt = 0;
		}
		else
		{
			playerX.SetAnimation( PlayerX::AnimationState::Idle, idleSpeed );
		}
	}
}
