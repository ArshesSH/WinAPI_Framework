#include "PlayerX.h"

#include "Scene.h"

void PlayerX::Update( float dt, Scene& scene )
{
	KbdInput(dt, scene);
	curAnimation.Update( dt, 0.3f );
}

void PlayerX::Draw( HDC hdc )
{
	curAnimation.PlayByCamGDI( hdc, sprite, Vec2<int>( GetPos() ), 2, chroma );

	Surface<int> surf;

	const auto pos = GetPos();
	const auto colliderPos = 
	const std::wstring imgPosStr = L"imgPos: (" + std::to_wstring( pos.x ) + L", " + std::to_wstring( pos.y ) + L")";
	surf.DrawStringGDI( hdc, { 0,0 }, imgPosStr );
}


