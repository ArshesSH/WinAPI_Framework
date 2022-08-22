#pragma once

#include "Enemy.h"

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
	class ElectricAttack;

public:
	SigmaHead(const Vec2<float>& pos )
		:
		Enemy( pos, { 0.0f, 0.0f }, colliderHalfWidth, colliderHalfHeight, moveSpeed,
			L"Images/RockmanX5/Sigma/SigmaHead.bmp", L"Images/RockmanX5/Sigma/SigmaHead.bmp" ),
		hitCollider(RectF::FromCenter(pos, hitColX, hitColY ))
	{
		animationMap[(int)AnimationState::Idle] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/Sigma/SigmaIdle.anim" );
		animationMap[(int)AnimationState::Ready] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/Sigma/SigmaReady.anim" );
		animationMap[(int)AnimationState::Fade] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/Sigma/SigmaFade.anim" );
		animationMap[(int)AnimationState::Open] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/Sigma/SigmaOpen.anim" );
		animationMap[(int)AnimationState::Damaged] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/Sigma/SigmaDamaged.anim" );

		SetAnimation( AnimationState::Idle, animPlaySpeed );
		SetInvincible();

	}

	void Update( float dt, class Scene& scene ) override;
	void Draw( HDC hdc ) override;
	void ApplyDamage( int damage ) override;

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
		isInvincible = true;
		isImune = true;
	}

private:
	static constexpr float colliderHalfWidth = 120.0f;
	static constexpr float colliderHalfHeight = 100.0f;
	static constexpr float moveSpeed = 200.0f;
	static constexpr float colliderRelPosX = 0.0f;
	static constexpr float colliderRelPosY = 0.0f;
	static constexpr float hitColX = 10.0f;
	static constexpr float hitColY = 30.0f;
	static constexpr float hitColRelPosX = -colliderHalfWidth;
	static constexpr float hitColRelPosY = hitColY - colliderHalfHeight;
	static constexpr COLORREF chroma = RGB( 255, 0, 255 );

	const Vec2<float> hitColRelPos = { hitColRelPosX, hitColRelPosY };

	int hp = 30;
	bool isInvincible = true;
	float animPlaySpeed = 0.0f;
	
	Animation<int> curAnimation;
	AnimationState curAnimState = AnimationState::Idle;
	ConvexCollider<float> hitCollider;

	std::unique_ptr<Behavior> pBehavior;
};