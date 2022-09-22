#pragma once

#include "Bullet.h"

class SigmaElectricBall : public Bullet
{
public:
	enum class Type
	{
		Default
	};

public:
	SigmaElectricBall(const Vec2<float>& pos, float speed, const Vec2<float>& dir  )
		:
		Bullet( ActorTag::EnemyBullet, ActorTag::Player, pos, { 0.0f,0.0f }, { ballSize ,ballSize }, speed, dir, damageSet,
			L"Images/RockmanX5/Sigma/SigmaHead.bmp", L"Images/RockmanX5/Sigma/SigmaHead.bmp" )
	{
		animationMap[(int)Type::Default] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/Sigma/Attack/SigmaElectricBall.anim" );
		curAnimation = animationMap[(int)Type::Default];
	}

	void Update( float dt, class Scene& scene ) override;
	void SetTransform( const Mat3<float>& transform ) override;
	void Draw( HDC hdc ) override;
	int GetHP() const override { return 0; }

	void SetVel( const Vec2<float>& vel_in )
	{
		vel = vel_in;
	}
	Vec2<float> GetDirToPlayer( class Scene& scene ) const;

public:
	static constexpr float lifeTime = 5.0f;
	float time = 0.0f;

private:
	static constexpr float animSpeed = 0.03f;
	static constexpr float ballSize = 30.0f;
	static constexpr int damageSet = 3;
	static constexpr float attackDelayTime = 1.0f;

	static constexpr COLORREF chroma = RGB( 255, 0, 255 );

	Animation<int> curAnimation;
	bool isDirSet = false;
	bool isDamgeOn = false;
};