#include "PlayerX.h"

#include "Scene.h"

void PlayerX::Update( float dt, Scene& scene )
{
	KbdInput();
}

void PlayerX::Draw( HDC hdc )
{
	GetCurAnimation().PlayGDI( hdc, sprite, Vec2<int>( GetPos() + imgHalfSize ), 1, chroma );
}


