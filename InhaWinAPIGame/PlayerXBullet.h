#pragma once

#include "Bullet.h"

class PlayerXBullet : public Bullet
{
public:
	enum class Type
	{
		Bullet1,
		Bullet2,
		Bullet3,
		HitNormal,
		HitCharged,
		HitImuned,
		Bullet3Remain,
		Bullet3Destroy,
		Size
	};

public:
	PlayerXBullet( Type type, const Vec2<float>& dir, const Vec2<float>& pivotPos, const Vec2<float>& colliderRelativePos, bool isFacingRight )
		:
		type( type ),
		isFacingRight( isFacingRight ),
		Bullet( ActorTag::PlayerBullet, ActorTag::Enemy, pivotPos, colliderRelativePos, { bullet1Width, bullet1Height }, bulletMoveSpeed, dir, 3,
			L"Images/RockmanX5/X/Bullet/Buster.bmp", L"Images/RockmanX5/X/Bullet/BusterFlip.bmp" )
	{
		animationMap[(int)Type::Bullet1] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Bullet/bullet1.anim" );
		animationMap[(int)Type::Bullet2] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Bullet/bullet2.anim" );
		animationMap[(int)Type::Bullet3] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Bullet/bullet3.anim" );
		animationMap[(int)Type::HitNormal] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Bullet/HitNormal.anim" );
		animationMap[(int)Type::HitCharged] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Bullet/HitCharged.anim" );
		animationMap[(int)Type::HitImuned] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Bullet/HitImuned.anim" );
		animationMap[(int)Type::Bullet3Remain] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Bullet/Bullet3Remain.anim" );
		animationMap[(int)Type::Bullet3Destroy] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Bullet/Bullet3Destroy.anim" );
	}
	void Update( float dt, class Scene& scene ) override;
	void SetTransform( const Mat3<float>& transform ) override;
	void Draw( HDC hdc ) override;

	void ChangeAnimationToHit();

	bool CheckHitAnimationFinish() const;


private:
	static constexpr float bullet1AnimSpeed = 0.03f;
	static constexpr float bullet2AnimSpeed = 0.03f;
	static constexpr float bullet3AnimSpeed = 0.05f;
	static constexpr float bulletMoveSpeed = 800.0f;
	static constexpr float bullet1Width = 10.0f;
	static constexpr float bullet1Height = 10.0f;
	static constexpr float bullet2Width = 30.0f;
	static constexpr float bullet2Height = 30.0f;
	static constexpr float bullet3Width = 100.0f;
	static constexpr float bullet3Height = 100.0f;

	Animation<int> curAnimation;
	Type animationState = Type::Size;
	Type type;
	bool isFacingRight;
	const COLORREF chroma = RGB( 255, 0, 255 );
	float animSpeed = 0.0f;

	bool isPlayHit = false;
};