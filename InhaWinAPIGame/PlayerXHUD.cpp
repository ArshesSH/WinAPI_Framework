#include "PlayerXHUD.h"

#include "Scene.h"

void PlayerXHUD::Update( float dt, Scene& scene )
{
	const int hp = (scene.FindPlayerPtr()->GetHP());
	
	if ( hp != hpSegmentPositions.size() )
	{
		for ( size_t i = hp; i < hpSegmentPositions.size(); ++i )
		{
			hpSegmentPositions.pop_back();
		}
	}


	UpdateLifeCount( dt, scene.GetPlayerLife() );

	hudAnimation.Update( dt, 0.0f );
	
	const auto transform = scene.AccessCamera().GetTransform();
}

void PlayerXHUD::Draw( HDC hdc )
{
	Gdiplus::Graphics gfx( hdc );
	hudAnimation.PlayGDI( hdc, image, pos - Vec2<int>( 2, 0 ), 4, chroma );
	DrawLifeCount( hdc );
	//surf.DrawImageNonChromaGDI( hdc, hpBarImage, healthRect.GetTopLeft(), { healthRect.GetWidth(), healthRect.GetHeight() }, { 0,0 }, { 10,10 } );
	DrawHpBar( hdc );
}
