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
		SetRandomAnimation();
	}
	void TransferFrom( PlayerX::State state_in ) override
	{
		if ( state_in != PlayerX::State::Idle )
		{
			playerX.SetState( PlayerX::State::Idle );
			SetRandomAnimation();
		}

		/*
		switch ( state_in )
		{
		case PlayerX::State::Intro:
			break;
		case PlayerX::State::Idle:
			break;
		case PlayerX::State::IdleBlink:
			break;
		case PlayerX::State::IdleLowHP:
			break;
		case PlayerX::State::Shoot:
			break;
		case PlayerX::State::ShootCharged:
			break;
		case PlayerX::State::WalkStart:
			break;
		case PlayerX::State::WalkLoop:
			break;
		case PlayerX::State::ShootWalkStart:
			break;
		case PlayerX::State::ShootWalkLoop:
			break;
		case PlayerX::State::DashStart:
			break;
		case PlayerX::State::DashLoop:
			break;
		case PlayerX::State::DashEnd:
			break;
		case PlayerX::State::ShootDashStart:
			break;
		case PlayerX::State::ShootDashLoop:
			break;
		case PlayerX::State::ShootDashEnd:
			break;
		case PlayerX::State::Jump:
			break;
		case PlayerX::State::Land:
			break;
		case PlayerX::State::ShootJump:
			break;
		case PlayerX::State::ShootLand:
			break;
		case PlayerX::State::WallCling:
			break;
		case PlayerX::State::WallKick:
			break;
		case PlayerX::State::ShootWallCling:
			break;
		case PlayerX::State::ShootWallKick:
			break;
		case PlayerX::State::LadderClimb:
			break;
		case PlayerX::State::LadderTop:
			break;
		case PlayerX::State::LadderShoot:
			break;
		case PlayerX::State::AirDashStart:
			break;
		case PlayerX::State::Hover:
			break;
		case PlayerX::State::HoverShoot:
			break;
		case PlayerX::State::HurtA:
			break;
		case PlayerX::State::HurtB:
			break;
		case PlayerX::State::HurtC:
			break;
		case PlayerX::State::Victory:
			break;
		case PlayerX::State::Exit:
			break;
		default:
			break;
		}
		*/
	}

private:
	void SetRandomAnimation()
	{
		const auto curAnim = playerX.GetAnimation();

		if ( curAnim.IsEnd() )
		{
			if ( playerX.GetState() == PlayerX::State::Idle )
			{
				idlePlayCnt++;
			}

			if ( idlePlayCnt == idleMaxCount )
			{
				playerX.SetAnimation( PlayerX::State::IdleBlink, idleBlinkSpeed );
				idlePlayCnt = 0;
			}
			else
			{
				playerX.SetAnimation( PlayerX::State::Idle, idleSpeed );
			}
		}
	}

private:
	static constexpr float idleSpeed = 0.3f;
	static constexpr float idleBlinkSpeed = 0.11f;
	static constexpr int idleMaxCount = 3;

	int idlePlayCnt = 0;
};