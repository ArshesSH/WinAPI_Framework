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
	void SetTransform( const Mat3<float>& transform ) override
	{
		curAnimation.SetTransform( transform );
	}
private:
	void KbdInput( float dt, class Scene& scene );


	Animation<int> GetCurAnimation() const
	{
		return animationMap.at( (int)state );
	}

private:
	static constexpr float colliderHalfWidth = 20.0f;
	static constexpr float colliderHalfHeight = 40.0f;
	static constexpr float defaultSpeed = 100.0f;
	static constexpr COLORREF chroma = RGB( 84, 165, 75 );

	State state = State::Idle;
	Animation<int> curAnimation;

	// Debug
	PivotGizmo pivotGizmo;
	std::wstring imgPosStr;
	std::wstring colliderPosStr;
};
