#include "PlayerX.h"

#include "Scene.h"


void PlayerX::Update( float dt, Scene& scene )
{
	KbdInput(dt, scene);
	curAnimation.Update( dt, 0.3f );
	
	// Debug
	const auto pos = GetPos();
	const auto colliderPos = GetColliderPos();
	imgPosStr = L"imgPos: (" + std::to_wstring( pos.x ) + L", " + std::to_wstring( pos.y ) + L")";
	colliderPosStr = L"colliderPos : (" + std::to_wstring( colliderPos.x ) + L", " + std::to_wstring( colliderPos.y ) + L")";

	pivotGizmo.SetPos( Vec2<int>( pos ) );
	pivotGizmo.SetTransform( scene.AccessCamera().GetTransform() );
}

void PlayerX::Draw( HDC hdc )
{
	curAnimation.PlayByCamGDI( hdc, sprite, Vec2<int>( GetPos() ), 2, chroma );


	// Debug
	Surface<int> surf;
	
	surf.DrawStringGDI( hdc, { 0,0 }, imgPosStr );
	surf.DrawStringGDI( hdc, { 0,20 }, colliderPosStr );
	
	pivotGizmo.Draw( hdc );
}


