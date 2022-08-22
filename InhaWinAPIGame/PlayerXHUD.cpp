#include "PlayerXHUD.h"

#include "Scene.h"

void PlayerXHUD::Update( float dt, Scene& scene )
{
	const int hp = (scene.FindPlayerPtr()->GetHP());
	const float percent = 204 / (float)hp;

	healthRect = { healthRect.GetTopLeft(), healthRect.GetBottomRight() - Vec2<int>{0, (maxHP - hp) * (int)percent} };

	UpdateLifeCount( dt, scene.GetPlayerLife() );

	hudAnimation.Update( dt, 0.0f );
	
	const auto transform = scene.AccessCamera().GetTransform();
}

void PlayerXHUD::Draw( HDC hdc )
{
	Gdiplus::Graphics gfx( hdc );
	hudAnimation.PlayGDI( hdc, image, pos - Vec2<int>( 2, 0 ), 4, chroma );
	DrawLifeCount( hdc );
	surf.DrawImageNonChromaGDI( hdc, hpBarImage, healthRect.GetTopLeft(), { healthRect.GetWidth(), healthRect.GetHeight() }, { 0,0 }, { 10,10 } );
}
