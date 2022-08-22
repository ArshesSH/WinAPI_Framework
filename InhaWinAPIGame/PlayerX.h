#pragma once

#include "Character.h"
#include "Image.h"
#include "PivotGizmo.h"
#include "Gravity.h"
#include "Keyboard.h"

#include "PlayerXBullet.h"

#ifndef NDBUG
#include <iostream>
#endif // !NDBUG

class PlayerX : public Character
{
public:
	enum class AnimationState
	{
		Intro,
		Idle,
		IdleBlink,
		IdleLowHP,
		Shoot,
		ShootEnd,
		ShootCharged,
		WalkStart,
		WalkLoop,
		ShootWalkStart,
		ShootWalkLoop,
		DashStart,
		DashLoop,
		DashEnd,
		ShootDashStart,
		ShootDashLoop,
		ShootDashEnd,
		Jump,
		Airbone,
		Land,
		ShootJump,
		ShootLand,
		WallSlide,
		WallCling,
		WallKick,
		ShootWallCling,
		ShootWallKick,
		Crouch,
		CrouchShootStart,
		CrouchShoot,
		CrouchShootCharged,
		LadderClimb,
		LadderTop,
		LadderShoot,
		AirDashStart,
		Hover,
		HoverFront,
		HoverBack,
		HoverShoot,
		HurtA,
		HurtB,
		HurtC,
		Victory,
		Exit
	};

public:
	enum class AttackState
	{
		NoAttack,
		Charge,
		Shoot,
		ShootMid,
		ShootMax,
		Hurt
	};
	enum class MoveState
	{
		Idle,
		Walk,
		Dash,
		AirDash,
		Jump,
		DashJump,
		Airbone,
		Hover,
		Land,
		Ladder,
		WallSlide,
		WallKick,
		Crouch
	};
	enum class ChargeState
	{
		NoCharge,
		Normal,
		Middle,
		Max
	};

public:
	using Behavior = Actor::Behavior<PlayerX>;
	class Idle;
	class Walk;
	class Dash;
	class AirDash;
	class Jump;
	class DashJump;
	class Airbone;
	class Hover;
	class WallSlide;
	class WallKick;
	class Crouch;

	class Shoot;

public:
	PlayerX( const Vec2<float>& pivotPos, const Vec2<float>& colliderRelativePos = { 0.0f, 40.0f } );

	void Update( float dt, class Scene& scene ) override;
	void Draw( HDC hdc ) override;
	void ApplyDamage( int damage ) override;

	void SetTransform( const Mat3<float>& transform ) override
	{
		curAnimation.SetTransform( transform );
	}
	const Animation<int>& GetAnimation() const
	{
		return curAnimation;
	}
	AnimationState GetAnimationState() const
	{
		return curAnimState;
	}
	void SetAnimation( AnimationState state, float animSpeed_in )
	{
		curAnimState = state;
		curAnimation = animationMap[(int)state];
		animPlaySpeed = animSpeed_in;
	}
	void SetAnimationPlaySpeed( float speed )
	{
		animPlaySpeed = speed;
	}

	void SetFacingRight( bool faceRight = true)
	{
		isFacingRight = faceRight;
	}
	bool IsFacingRight() const
	{
		return isFacingRight;
	}

	Vec2<float> GetVel() const
	{
		return vel;
	}
	void SetVel( const Vec2<float>& vel_in )
	{
		vel = vel_in;
	}
	void SetMoveSpeed(float speed)
	{
		moveSpeed = speed;
	}
	void SetStopFacingTrack( bool notFacing = true )
	{
		isStopFacingTrack = notFacing;
	}

	void SetDashEnd()
	{
		isDashEnd = true;
	}

	bool CheckMoveStateChange() const 
	{
		return (moveState != oldMoveState);
	}
	bool CheckAttackStateChange() const
	{
		return  (attackState != oldAttackState);
	}

	void UpdatePlayerState();
	void UpdatePlayerBehavior();
	

#ifndef NDEBUG
	void DrawStateString(Surface<int>& surf, HDC hdc)
	{
		switch ( moveState )
		{
		case PlayerX::MoveState::Idle:
			moveStateStr = L"MoveState = Idle";
			break;
		case PlayerX::MoveState::Walk:
			moveStateStr = L"MoveState = Walk";
			break;
		case PlayerX::MoveState::Dash:
			moveStateStr = L"MoveState = Dash";
			break;
		case PlayerX::MoveState::AirDash:
			moveStateStr = L"MoveState = AirDash";
			break;
		case PlayerX::MoveState::Jump:
			moveStateStr = L"MoveState = Jump";
			break;
		case PlayerX::MoveState::DashJump:
			moveStateStr = L"MoveState = DashJump";
			break;
		case PlayerX::MoveState::Airbone:
			moveStateStr = L"MoveState = Airbone";
			break;
		case PlayerX::MoveState::Hover:
			moveStateStr = L"MoveState = Hover";
			break;
		case PlayerX::MoveState::Land:
			moveStateStr = L"MoveState = Land";
			break;
		case PlayerX::MoveState::Ladder:
			moveStateStr = L"MoveState = Ladder";
			break;
		case PlayerX::MoveState::WallSlide:
			moveStateStr = L"MoveState = WallSlide";
			break;
		case PlayerX::MoveState::WallKick:
			moveStateStr = L"MoveState = WallKick";
			break;
		case PlayerX::MoveState::Crouch:
			moveStateStr = L"MoveState = Crouch";
			break;
		default:
			break;
		}
		switch ( attackState )
		{
		case PlayerX::AttackState::NoAttack:
			attackStateStr = L"AttackState = NoAttack";
			break;
		case PlayerX::AttackState::Charge:
			attackStateStr = L"AttackState = Charge";
			break;
		case PlayerX::AttackState::Shoot:
			attackStateStr = L"AttackState = Shoot";
			break;
		case PlayerX::AttackState::ShootMid:
			attackStateStr = L"AttackState = ShootMid";
			break;
		case PlayerX::AttackState::ShootMax:
			attackStateStr = L"AttackState = ShootMax";
			break;
		case PlayerX::AttackState::Hurt:
			attackStateStr = L"AttackState = Hurt";
			break;
		default:
			break;
		}

		surf.DrawStringGDI( hdc, { 0, 40 }, moveStateStr );
		surf.DrawStringGDI( hdc, { 0, 60 }, attackStateStr );
	}

	void DrawAnimationStateString( Surface<int>& surf, HDC hdc)
	{
		switch ( curAnimState )
		{
		case PlayerX::AnimationState::Intro:
			animStateStr = L"Intro";
			break;
		case PlayerX::AnimationState::Idle:
			animStateStr = L"Idle";
			break;
		case PlayerX::AnimationState::IdleBlink:
			animStateStr = L"IdleBlink";
			break;
		case PlayerX::AnimationState::IdleLowHP:
			animStateStr = L"IdleLowHP";
			break;
		case PlayerX::AnimationState::Shoot:
			animStateStr = L"Shoot";
			break;
		case PlayerX::AnimationState::ShootEnd:
			animStateStr = L"ShootEnd";
			break;
		case PlayerX::AnimationState::ShootCharged:
			animStateStr = L"ShootCharged";
			break;
		case PlayerX::AnimationState::WalkStart:
			animStateStr = L"WalkStart";
			break;
		case PlayerX::AnimationState::WalkLoop:
			animStateStr = L"WalkLoop";
			break;
		case PlayerX::AnimationState::ShootWalkStart:
			animStateStr = L"ShootWalkStart";
			break;
		case PlayerX::AnimationState::ShootWalkLoop:
			animStateStr = L"ShootWalkLoop";
			break;
		case PlayerX::AnimationState::DashStart:
			animStateStr = L"DashStart";
			break;
		case PlayerX::AnimationState::DashLoop:
			animStateStr = L"DashLoop";
			break;
		case PlayerX::AnimationState::DashEnd:
			animStateStr = L"DashEnd";
			break;
		case PlayerX::AnimationState::ShootDashStart:
			animStateStr = L"ShootDashStart";
			break;
		case PlayerX::AnimationState::ShootDashLoop:
			animStateStr = L"ShootDashLoop";
			break;
		case PlayerX::AnimationState::ShootDashEnd:
			animStateStr = L"ShootDashEnd";
			break;
		case PlayerX::AnimationState::Jump:
			animStateStr = L"Jump";
			break;
		case PlayerX::AnimationState::Land:
			animStateStr = L"Land";
			break;
		case PlayerX::AnimationState::ShootJump:
			animStateStr = L"ShootJump";
			break;
		case PlayerX::AnimationState::ShootLand:
			animStateStr = L"ShootLand";
			break;
		case PlayerX::AnimationState::WallSlide:
			animStateStr = L"WallSlide";
			break;
		case PlayerX::AnimationState::WallCling:
			animStateStr = L"WallCling";
			break;
		case PlayerX::AnimationState::WallKick:
			animStateStr = L"WallKick";
			break;
		case PlayerX::AnimationState::ShootWallCling:
			animStateStr = L"ShootWallCling";
			break;
		case PlayerX::AnimationState::ShootWallKick:
			animStateStr = L"ShootWallKick";
			break;
		case PlayerX::AnimationState::Crouch:
			animStateStr = L"Crouch";
			break;
		case PlayerX::AnimationState::CrouchShoot:
			animStateStr = L"CrouchShoot";
			break;
		case PlayerX::AnimationState::CrouchShootCharged:
			animStateStr = L"CrouchShootCharged";
			break;
		case PlayerX::AnimationState::LadderClimb:
			animStateStr = L"LadderClimb";
			break;
		case PlayerX::AnimationState::LadderTop:
			animStateStr = L"LadderTop";
			break;
		case PlayerX::AnimationState::LadderShoot:
			animStateStr = L"LadderShoot";
			break;
		case PlayerX::AnimationState::AirDashStart:
			animStateStr = L"AirDashStart";
			break;
		case PlayerX::AnimationState::HoverFront:
			animStateStr = L"HoverFront";
			break;
		case PlayerX::AnimationState::HoverBack:
			animStateStr = L"HoverBack";
			break;
		case PlayerX::AnimationState::HoverShoot:
			animStateStr = L"HoverShoot";
			break;
		case PlayerX::AnimationState::HurtA:
			animStateStr = L"HurtA";
			break;
		case PlayerX::AnimationState::HurtB:
			animStateStr = L"HurtB";
			break;
		case PlayerX::AnimationState::HurtC:
			animStateStr = L"HurtC";
			break;
		case PlayerX::AnimationState::Victory:
			animStateStr = L"Victory";
			break;
		case PlayerX::AnimationState::Exit:
			animStateStr = L"Exit";
			break;
		default:
			break;
		}
		surf.DrawStringGDI( hdc, { 0, 80 }, animStateStr );
	}


#endif // !NDEBUG


private:
	void KbdInput( float dt, Scene& scene );
	void TestKbd(float dt, Scene& scene);
	void UpdateWallSearcher(float dt);
	bool IsWallSearcherCollide( Scene& scene );
	void SpawnBullet( PlayerXBullet::Type type, Scene& scene, const Vec2<float>& relativeSpawnPos );
private:
	static constexpr float colliderHalfWidth = 20.0f;
	static constexpr float colliderHalfHeight = 40.0f;
	static constexpr COLORREF chroma = RGB( 84, 165, 75 );
	static constexpr float defaultMoveSpeed = 250.0f;
	static constexpr float dashSpeed = 400.0f;
	static constexpr float jumpSpeed = 350.0f;
	static constexpr float wallsearcherLength = 21.0f;
	static constexpr float bulletSpawnDefaultX = 20.0f;
	static constexpr float bulletSpawnDefaultY = 60.0f;
	static constexpr float bulletSpawnDashX = 50.0f;
	static constexpr float bulletSpawnDashY = 35.0f;
	static constexpr float bulletChargeMiddle = 0.5f;
	static constexpr float bulletChargeMax = 1.5f;
	static constexpr float busterMinDelay = 0.3f;
	static constexpr float chargeAnimSpeed = 0.1f;
	static constexpr float chargeFinAnimSpeed = 0.05f;

	AnimationState curAnimState;
	Animation<int> curAnimation;
	float animPlaySpeed;

	LineCollider<float> wallSearcher;
	const Vec2<float> wallSearcherOffset = { 0.0f,1.0f };
	int hp = 20;

	Image::ImageGDI<int> chargeImage;
	Animation<int> chargeAnimation;
	Animation<int> chargeFinAnim;
	const Vec2<float> chargeOffset = { 0.0f, 50.0f };

	// Character Statement
	AttackState attackState = AttackState::NoAttack;
	AttackState oldAttackState = attackState;
	MoveState moveState = MoveState::Idle;
	MoveState oldMoveState = moveState;
	bool isAttackStateChanged = false;
	bool isMoveStateChanged = false;

	bool isStopFacingTrack = false;
	bool isOnGround = false;
	bool isJumpNow = false;
	bool isDashEnd = false;
	bool isJumpEnd = false;
	bool isOnWallSide = false;
	bool canAirDash = false;

	float chargeTime = 0.0f;
	ChargeState chargeState = ChargeState::NoCharge;

	// Key Statement
	Keyboard kbd;
	bool isRightKeyDown = false;
	bool isLeftKeyDown = false;
	bool isDownKeyDown = false;
	bool isZKeyDown = false;
	bool isXKeyDown = false;
	bool isCKeyDown = false;
	bool isESCKeyDown = false;
	bool isEnterkeyDown = false;
	bool isXKeyInputOnce = false;
	bool isCKeyInputOnce = false;
	int hoverCount = 0;

	std::unique_ptr<Behavior> pBehavior;
	Gravity gravity;

#ifndef NDEBUG
	// Debug
	PivotGizmo pivotGizmo;
	std::wstring imgPosStr;
	std::wstring colliderPosStr;
	std::wstring isRightKeyStr;
	std::wstring isLeftKeyStr;
	std::wstring moveStateStr;
	std::wstring attackStateStr;
	std::wstring animStateStr;
#endif
};
