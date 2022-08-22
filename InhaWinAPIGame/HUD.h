#pragma once

#include "Animation.h"

class HUD
{
public:

	HUD( int maxHP, const Vec2<int>& pos, const Vec2<int>& numPos, const Vec2<int>& tlPos, const Vec2<int>& brPos, const std::wstring& fileName, COLORREF chroma )
		:
		maxHP( maxHP ),
		pos(pos),
		numPos(numPos),
		healthRect( pos + tlPos,  pos +brPos ),
		image( fileName ),
		chroma(chroma),
		lifeCountLeft( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/UI/HudNumbers.anim" ),
		lifeCountRight( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/UI/HudNumbers.anim" ),
		hpBarImage( L"Images/RockmanX5/UI/hpBar.bmp")
	{}

	virtual ~HUD() = default;

	virtual void Update(float dt, class Scene& scene) = 0;
	virtual void Draw( HDC hdc ) = 0;
	void UpdateLifeCount( float dt, int num )
	{
		int leftNum = num % 10;
		int rightNum = num / 10;

		lifeCountLeft.SetFrameIndex( leftNum - 1 );
		lifeCountRight.SetFrameIndex( rightNum - 1 );
		lifeCountLeft.Update( dt, 0.0f );
		lifeCountRight.Update( dt, 0.0f );


	}
	void DrawLifeCount( HDC hdc )
	{
		lifeCountLeft.PlayGDI( hdc, image, numPos - Vec2<int>( -2, 0 ), 4, chroma );
		lifeCountRight.PlayGDI( hdc, image, numPos - Vec2<int>( 2, 0 ), 4, chroma );
	}

protected:
	Surface<int> surf;
	const int maxHP;
	const COLORREF chroma;
	Vec2<int> pos;
	Vec2<int> numPos;
	Image::ImageGDI<int> image;
	Image::ImageGDI<int> hpBarImage;
	Animation<int> hudAnimation;
	Animation<int> lifeCountLeft;
	Animation<int> lifeCountRight;
	RectI healthRect;
};