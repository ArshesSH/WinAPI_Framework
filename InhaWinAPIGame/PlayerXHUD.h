#pragma once

#include "HUD.h"

class PlayerXHUD : public HUD
{
public:
	PlayerXHUD( int maxHP, const Vec2<int>& pos )
		:
		HUD( maxHP, pos, pos + Vec2<int>{0, numPosOffset}, Vec2<int>{ startPosX, startPosY }, Vec2<int>{ endPosX, endPosY  },
			L"Images/RockmanX5/UI/MMX5HUD.bmp", chroma )
	{
		hudAnimation = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/UI/HudX.anim" );
	}

	void Update( float dt, class Scene& scene ) override;
	void Draw(HDC hdc) override;

private:
	static constexpr COLORREF chroma = RGB( 255, 0, 255 );
	static constexpr int startPosX = 48;
	static constexpr int startPosY = 16;
	static constexpr int endPosY = 204;
	static constexpr int endPosX = startPosX + 20;
	static constexpr int numPosOffset = -10;
	
};