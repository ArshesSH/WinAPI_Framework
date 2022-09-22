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
			vKeyDownStatetable[vKey] = curKeyState;
		}

		if ( curKeyState == true && vKeyDownStatetable[vKey] == false )
		{
			vKeyDownStatetable[vKey] = curKeyState;
			return true;
		}
		return false;
	}

	bool IsKeyUpOccur( int vKey )
	{
		bool curKeyState = GetAsyncKeyState( vKey ) & 0x8000;


		if ( curKeyState == false && vKeyUpStatetable[vKey] == true )
		{
			vKeyUpStatetable[vKey] = curKeyState;
			return true;
		}
		if ( curKeyState )
		{
			vKeyUpStatetable[vKey] = curKeyState;
		}

		return false;
	}

private:
	std::unordered_map<int, bool> vKeyDownStatetable;
	std::unordered_map<int, bool> vKeyUpStatetable;
};
