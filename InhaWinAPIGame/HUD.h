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
		image( fileName ),
		chroma(chroma),
		lifeCountLeft( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/UI/HudNumbers.anim" ),
		lifeCountRight( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/UI/HudNumbers.anim" ),
		hpBarImage( L"Images/RockmanX5/UI/hpBar.bmp"),
		eachSegmentHeight(int( (std::abs( (brPos - tlPos).y ) / maxHP ) + 1 ) ),
		eachSegmentWidth( std::abs( (brPos - tlPos).x ))
	{
		for ( auto i = 0; i < maxHP; ++i )
		{
			hpSegmentPositions.emplace_back( brPos.x, brPos.y - eachSegmentHeight * i + 104);
		}
		
	}

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

	void DrawHpBar( HDC hdc )
	{
		for ( const auto& hpBar : hpSegmentPositions )
		{
			surf.DrawImageNonChromaGDI( hdc, hpBarImage, hpBar, { eachSegmentWidth, eachSegmentHeight }, { 0,0 }, { 10,10 } );
		}
	}

protected:
	Surface<int> surf;
	const int maxHP;
	const int eachSegmentHeight;
	const int eachSegmentWidth;
	const COLORREF chroma;
	Vec2<int> pos;
	Vec2<int> numPos;
	Image::ImageGDI<int> image;
	Image::ImageGDI<int> hpBarImage;
	Animation<int> hudAnimation;
	std::vector<Vec2<int>> hpSegmentPositions;
	
	Animation<int> lifeCountLeft;
	Animation<int> lifeCountRight;
	RectI healthRect;
};