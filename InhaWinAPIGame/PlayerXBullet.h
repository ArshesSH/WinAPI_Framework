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
		Size
	};

public:
	PlayerXBullet( Type type, const Vec2<float>& dir, const Vec2<float>& pivotPos, const Vec2<float>& colliderRelativePos, bool isFacingRight )
		:
		type(type),
		isFacingRight(isFacingRight),
		Bullet( ActorTag::PlayerBullet, ActorTag::Enemy, pivotPos, colliderRelativePos, { bullet1Width, bullet1Height }, bulletMoveSpeed, dir, 3,
			L"Images/RockmanX5/X/Bullet/Buster.bmp", L"Images/RockmanX5/X/Bullet/BusterFlip.bmp" )
	{
		animationMap[(int)Type::Bullet1] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Bullet/bullet1.anim" );
		animationMap[(int)Type::Bullet2] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Bullet/bullet2.anim" );
		animationMap[(int)Type::Bullet3] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Bullet/bullet3.anim" );
	}
	void Update( float dt, class Scene& scene ) override
	{
		switch ( type )
		{
		case PlayerXBullet::Type::Bullet1:
			if ( animationState != Type::Bullet1 )
			{
				damage = 3;
				animationState = Type::Bullet1;
				curAnimation = animationMap[(int)Type::Bullet1];
				animSpeed = bullet1AnimSpeed;

			}
			break;
		case PlayerXBullet::Type::Bullet2:
			if ( animationState != Type::Bullet2 )
			{
				damage = 4;
				animationState = Type::Bullet2;
				curAnimation = animationMap[(int)Type::Bullet2];
				animSpeed = bullet2AnimSpeed;
				this->ChangeColliderSize( { bullet2Width, bullet2Height } );
			}

			break;
		case PlayerXBullet::Type::Bullet3:
			if ( animationState != Type::Bullet3 )
			{
				damage = 8;
				animationState = Type::Bullet3;
				curAnimation = animationMap[(int)Type::Bullet3];
				animSpeed = bullet3AnimSpeed;
				this->ChangeColliderSize( { bullet3Width, bullet3Height } );
			}
			break;
		}
		if ( curAnimation.IsEnd() )
		{
			curAnimation.SetStop();
		}
		curAnimation.Update( dt, animSpeed );
		Bullet::Update( dt, scene );
	}
	void SetTransform( const Mat3<float>& transform ) override
	{
		curAnimation.SetTransform( transform );
	}
	void Draw( HDC hdc ) override
	{
		if ( !isFacingRight )
		{
			curAnimation.PlayByCamGDI( hdc, spriteFlipped, Vec2<int>( GetPos() ), 2, chroma, true );
		}
		else
		{
			curAnimation.PlayByCamGDI( hdc, sprite, Vec2<int>( GetPos() ), 2, chroma );
		}
	}

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
};