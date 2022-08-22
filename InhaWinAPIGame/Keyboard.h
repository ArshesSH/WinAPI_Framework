#pragma once

#include "framework.h"
#include <unordered_map>

class Keyboard
{
public:

	bool IsKeyPressed( int vKey )
	{
		return GetAsyncKeyState( vKey ) & 0x8000;
	}

	bool IsKeyDownOccur( int vKey )
	{
		bool curKeyState = GetAsyncKeyState( vKey ) & 0x8000;

		if ( !curKeyState )
		{
			vKeyStatetable[vKey] = curKeyState;
		}

		if ( curKeyState == true && vKeyStatetable[vKey] == false )
		{
			vKeyStatetable[vKey] = curKeyState;
			return true;
		}
		return false;
	}

	bool IsKeyUpOccur( int vKey )
	{
		bool curKeyState = GetAsyncKeyState( vKey ) & 0x8000;

		if ( curKeyState )
		{
			vKeyStatetable[vKey] = curKeyState;
		}

		if ( curKeyState == false && vKeyStatetable[vKey] == true )
		{
			vKeyStatetable[vKey] = curKeyState;
			return true;
		}
		return false;
	}

private:
	std::unordered_map<int, bool> vKeyStatetable;
};
