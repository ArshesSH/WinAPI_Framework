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

private:
	const std::wstring imageName = L"Images/awsom.bmp";
};