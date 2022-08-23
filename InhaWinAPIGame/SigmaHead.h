#pragma once

#include "Enemy.h"
#include <random>

class SigmaHead : public Enemy
{
public:
	enum class AnimationState
	{
		Idle,
		Ready,
		Fade,
		Open,
		Damaged
	};

public:
	using Behavior = Actor::Behavior<SigmaHead>;
	class Ready;
	class Fade;
	class Decide;
	class Open;
	class Damaged;
	class ElectricAttack;

public:
	SigmaHead( const Vec2<float>& pos, std::mt19937& rng );


	void Update( float dt, class Scene& scene ) override;
	void Draw( HDC hdc ) override;
	void ApplyDamage( int damage ) override;
	void Move( float dt, const class Scene& scene ) override;
	int GetHP() const override
	{
		return hp;
	}

	void SetTransform( const Mat3<float>& transform ) override
	{
		curAnimation.SetTransform( transform );
	}
	void SetAnimation( AnimationState state, float animSpeed_in )
	{
		curAnimState = state;
		curAnimation = animationMap[(int)state];
		animPlaySpeed = animSpeed_in;
	}

	void SetInvincible( bool mode = true )
	{
		isInvincible = mode;
		isImune = mode;
	}

	bool IsDead() const
	{
		return hp < 0;
	}

private:
	static constexpr float colliderHalfWidth = 120.0f;
	static constexpr float colliderHalfHeight = 100.0f;
	static constexpr float moveSpeed = 350.0f;
	static constexpr float colliderRelPosX = 0.0f;
	static constexpr float colliderRelPosY = 0.0f;
	static constexpr float hitColX = 10.0f;
	static constexpr float hitColY = 30.0f;
	static constexpr float hitColRelPosX = -colliderHalfWidth;
	static constexpr float hitColRelPosY = hitColY - colliderHalfHeight;
	static constexpr COLORREF chroma = RGB( 255, 0, 255 );

	const Vec2<float> hitColRelPos = { hitColRelPosX, hitColRelPosY };

	int hp = 20;
	bool isInvincible = true;
	bool isDamaged = false;
	float animPlaySpeed = 0.0f;
	
	Animation<int> curAnimation;
	AnimationState curAnimState = AnimationState::Idle;
	ConvexCollider<float> hitCollider;

	std::unique_ptr<Behavior> pBehavior;
};