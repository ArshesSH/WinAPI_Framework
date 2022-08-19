#pragma once

#include "PlayerXBehavior.h"
#include <random>

class PlayerXIdle : public PlayerXBehavior
{
public:
	PlayerXIdle( PlayerX& playerX )
		:
		PlayerXBehavior( playerX)
	{}
	void Do( float dt, Scene& scene ) override
	{
		//SetRandomAnimation();
	}
	//void TransferFrom( PlayerX::State state_in ) override
	//{
	//	if ( state_in != PlayerX::State::Idle )
	//	{
	//		playerX.SetState( PlayerX::State::Idle );
	//		SetRandomAnimation();
	//	}

	//}

private:
	//void SetRandomAnimation()
	//{
	//	const auto curAnim = playerX.GetAnimation();

	//	if ( curAnim.IsEnd() )
	//	{
	//		if ( playerX.GetState() == PlayerX::State::Idle )
	//		{
	//			idlePlayCnt++;
	//		}

	//		if ( idlePlayCnt == idleMaxCount )
	//		{
	//			playerX.SetAnimation( PlayerX::State::IdleBlink, idleBlinkSpeed );
	//			idlePlayCnt = 0;
	//		}
	//		else
	//		{
	//			playerX.SetAnimation( PlayerX::State::Idle, idleSpeed );
	//		}
	//	}
	//}

private:
	static constexpr float idleSpeed = 0.3f;
	static constexpr float idleBlinkSpeed = 0.11f;
	static constexpr int idleMaxCount = 3;

	int idlePlayCnt = 0;
};