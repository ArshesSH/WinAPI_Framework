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
	PlayerX( const Vec2<float>& startPos )
		:
		Character( ActorTag::Player, RectF::FromCenter( startPos, colliderHalfWidth, colliderHalfHeight ) , defaultSpeed )
	{
		animationMap[(int)State::Idle] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Idle.anim" );
		animationMap[(int)State::IdleBlink] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/IdleBlink.anim" );
		animationMap[(int)State::WalkStart] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/WalkStart.anim" );
		animationMap[(int)State::WalkLoop] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/walkLoop.anim" );
	}

	void Update( float dt, class Scene& scene ) override;
	void Draw( HDC hdc ) override;

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

private:
	static constexpr float colliderHalfWidth = 15.0f;
	static constexpr float colliderHalfHeight = 50.0f;
	static constexpr float defaultSpeed = 10.0f;

	State state = State::Idle;
};
