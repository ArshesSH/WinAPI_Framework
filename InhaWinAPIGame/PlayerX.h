#pragma once

#include "Character.h"
#include "Image.h"

class PlayerX : public Character
{
public:
	enum class State
	{
		Idle,
		IdleBlink,
		WalkStart,
		WalkLoop,
	};

public:
	PlayerX( const Vec2<float>& startPos, const Vec2<float>& imgHalfSize )
		:
		Character( ActorTag::Player, RectF::FromCenter( startPos, colliderHalfWidth, colliderHalfHeight ), defaultSpeed,
			L"Images/RockmanX5/X/ForthArmorSprite.bmp", imgHalfSize)
	{
		animationMap[(int)State::Idle] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Idle.anim" );
		animationMap[(int)State::IdleBlink] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/IdleBlink.anim" );
		animationMap[(int)State::WalkStart] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/WalkStart.anim" );
		animationMap[(int)State::WalkLoop] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/walkLoop.anim" );


		curAnimation = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Idle.anim" );
	}

	void Update( float dt, class Scene& scene ) override;
	void Draw( HDC hdc ) override;
	void SetTransform( const Mat3<float>& transform ) override
	{
		curAnimation.SetTransform( transform );
	}
private:
	void KbdInput()
	{
		state = State::Idle;
		dir = { 0.0f, 0.0f };
		if ( GetAsyncKeyState( VK_LEFT ) & 0x8001 )
		{
			dir = dirLeft;
		}
		if ( GetAsyncKeyState( VK_RIGHT ) & 0x8001 )
		{
			dir = dirRight;
		}
	}

	void StateChangeToWalk()
	{
		switch ( state )
		{
		case PlayerX::State::Idle:
			break;
		case PlayerX::State::IdleBlink:
			break;
		case PlayerX::State::WalkStart:
			break;
		case PlayerX::State::WalkLoop:
			break;
		}
	}

	Animation<int> GetCurAnimation() const
	{
		return animationMap.at( (int)state );
	}

private:
	static constexpr float colliderHalfWidth = 25.0f;
	static constexpr float colliderHalfHeight = 25.0f;
	static constexpr float defaultSpeed = 100.0f;
	static constexpr COLORREF chroma = RGB( 84, 165, 75 );

	State state = State::Idle;
	Animation<int> curAnimation;
};
