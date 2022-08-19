#pragma once

#include "Character.h"
#include "Image.h"
#include "PivotGizmo.h"

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
	enum class State
	{
		Idle,
		Walk,
	};

public:
	using Behavior = Actor::Behavior<PlayerX>;
	class Idle;
	class WalkStart;
	class WalkLoop;

public:
	PlayerX( const Vec2<float>& pivotPos, const Vec2<float>& colliderRelativePos = { 0.0f, 40.0f } );
	//~PlayerX();

	void Update( float dt, class Scene& scene ) override;
	void Draw( HDC hdc ) override;

	State GetState() const
	{
		return curState;
	}
	void SetState( State state)
	{
		curState = state;
	}

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
		animSpeed = animSpeed_in;
	}

	void SetAnimationPlaySpeed( float speed )
	{
		animSpeed = speed;
	}
	void SetFacingRight( bool faceRight = true)
	{
		isFacingRight = faceRight;
	}
	bool IsFacingRight() const
	{
		return isFacingRight;
	}

	void Walk( float dt, Scene& scene );


private:
	void KbdInput( float dt, class Scene& scene );

private:
	static constexpr float colliderHalfWidth = 20.0f;
	static constexpr float colliderHalfHeight = 40.0f;
	static constexpr float defaultSpeed = 200.0f;
	static constexpr COLORREF chroma = RGB( 84, 165, 75 );
	static constexpr float minJumpTime = 0.2f;
	static constexpr float maxJumpTime = 1.0f;

	State curState;
	AnimationState curAnimState;
	Animation<int> curAnimation;
	float animSpeed = 0.3f;
	bool isFacingRight = false;

	//std::unique_ptr<class PlayerXBehavior> pBehavior;

	std::unique_ptr<Behavior> pBehavior;

	// Debug
	PivotGizmo pivotGizmo;
	std::wstring imgPosStr;
	std::wstring colliderPosStr;
};
