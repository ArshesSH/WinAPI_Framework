#include "PlayerX.h"

#include "Scene.h"

void PlayerX::Update( float dt, Scene& scene )
{
	KbdInput();
	Move( dt, scene );
	curAnimation.Update( dt, 0.3f );
}

void PlayerX::Draw( HDC hdc )
{
	curAnimation.PlayGDI( hdc, sprite, Vec2<int>( GetPos() + imgHalfSize ), 2, chroma );


	DrawCollider( *(Gdiplus::Graphics::FromHDC( hdc )) );
}


