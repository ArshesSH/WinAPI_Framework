#pragma once

#include "PlayerXBehavior.h"

class PlayerXWalk : public PlayerXBehavior
{
public:
	PlayerXWalk( PlayerX& playerX )
		:
		PlayerXBehavior( playerX )
	{}
	void Do( float dt, Scene& scene ) override
	{
		TransferFrom( playerX.GetState() );
	}

	void TransferFrom( PlayerX::State state_in ) override
	{
		const auto curXState = playerX.GetState();
		const auto curXAnim = playerX.GetAnimation();
		switch ( state_in )
		{
		case PlayerX::State::Intro:
			break;
		case PlayerX::State::Idle:
		case PlayerX::State::IdleBlink:
			playerX.SetState( PlayerX::State::WalkStart );
			playerX.SetAnimation( PlayerX::State::WalkStart, walkStartSpeed );
			break;
		case PlayerX::State::IdleLowHP:
			break;
		case PlayerX::State::Shoot:
			break;
		case PlayerX::State::ShootCharged:
			break;
		case PlayerX::State::WalkStart:
			if ( curXAnim.IsEnd() )
			{
				playerX.SetState( PlayerX::State::WalkLoop );
				playerX.SetAnimation( PlayerX::State::WalkLoop, walkLoopSpeed );
			}
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
	}

private:

private:
	static constexpr float walkLoopSpeed = 0.3f;
	static constexpr float walkStartSpeed = 0.2f;
};