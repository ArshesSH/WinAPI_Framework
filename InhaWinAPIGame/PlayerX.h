#pragma once

#include "Character.h"
#include "Image.h"
#include "PivotGizmo.h"

class PlayerX : public Character
{
public:
	enum class State
	{
		Intro,
		Idle,
		IdleBlink,
		IdleLowHP,
		Shoot,
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
		Land,
		ShootJump,
		ShootLand,
		WallCling,
		WallKick,
		ShootWallCling,
		ShootWallKick,
		LadderClimb,
		LadderTop,
		LadderShoot,
		AirDashStart,
		Hover,
		HoverShoot,
		HurtA,
		HurtB,
		HurtC,
		Victory,
		Exit
	};

public:
	PlayerX( const Vec2<float>& pivotPos, const Vec2<float>& colliderRelativePos = { 0.0f, 40.0f } );

	void Update( float dt, class Scene& scene ) override;
	void Draw( HDC hdc ) override;


	State GetState() const
	{
		return state;
	}
	void SetState(State state_in)
	{
		state = state_in;
	}

	void SetTransform( const Mat3<float>& transform ) override
	{
		curAnimation.SetTransform( transform );
	}
	Animation<int> GetCurAnimationFromMap() const
	{
		return animationMap.at( (int)state );
	}
	const Animation<int>& GetAnimation() const
	{
		return curAnimation;
	}
	void SetAnimation( State state, float animSpeed_in )
	{
		curAnimation = animationMap[(int)state];
		animSpeed = animSpeed_in;
	}

	void SetAnimationPlaySpeed( float speed )
	{
		animSpeed = speed;
	}

private:
	void KbdInput( float dt, class Scene& scene );

private:
	static constexpr float colliderHalfWidth = 20.0f;
	static constexpr float colliderHalfHeight = 40.0f;
	static constexpr float defaultSpeed = 200.0f;
	static constexpr COLORREF chroma = RGB( 84, 165, 75 );

	State state = State::Idle;
	State prevState = State::Idle;
	Animation<int> curAnimation;
	float animSpeed = 0.3f;
	bool isFlipped = false;

	std::unique_ptr<class PlayerXBehavior> pBehavior;

	// Debug
	PivotGizmo pivotGizmo;
	std::wstring imgPosStr;
	std::wstring colliderPosStr;
};
