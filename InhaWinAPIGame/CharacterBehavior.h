#pragma once

#include "Character.h"

class CharacterBehavior
{
public:
	CharacterBehavior( Character& character )
		:
		character( character )
	{}


private:
	Character& character;
};

