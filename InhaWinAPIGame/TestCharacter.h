#pragma once

#include "Character.h"
#include <string>
#include "Game.h"

template <typename T>
class TestCharacter : public Character<T>
{
public:
	TestCharacter()
		:
		Character(imageName)
	{}
	void Update( float dt, Game& game ) override
	{

	}
	void Draw( HDC hdc ) override
	{
		
	}
	void Draw( Gdiplus::Graphics& gfx )
	{

	}
private:
	void Control( float dt ) override
	{

	}
	bool CanMove( const Vec2<T>& pos )override
	{

	}

private:
	const std::wstring imageName = L"Images/awsom.bmp";
};