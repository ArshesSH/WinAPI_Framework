#pragma once

#include <vector>
#include "Surface.h"
#include "Image.h"
#include "UtilSH.h"

template <typename T>
class Animation
{
public:
	enum class SpriteType
	{
		GDI,
		GDIPlus
	};

public:
	class Frame
	{
	public:
		Frame(){}
		Frame( const RectI& sprite, float ndcX, float ndcY )
			:
			sprite( sprite ),
			pivot( int( sprite.GetWidth() * ndcX ), int( sprite.GetHeight() * ndcY ) )
		{
		}
		Frame( const RectI& sprite, const Vec2<int>& pivot )
			:
			sprite( sprite ),
			pivot( pivot )
		{}
	public:
		RectI sprite;
		Vec2<int> pivot;
	};

public:
	Animation( SpriteType spriteType, const std::wstring& spritePath, const std::vector<Frame>& frames, float playSpeed )
		:
		spriteType(spriteType),
		spritePath(spritePath),
		frames(frames),
		playSpeed(playSpeed)
	{
	}
	void SetTransform( const Mat3<float>& transform )
	{
		surf.SetTransformation( transform );
	}
	void Update( float dt )
	{
		playTime += dt;
		if ( playTime >= playSpeed )
		{
			curIdx = UtilSH::GetSafeIndex( frames, curIdx + 1 );
			playTime = 0.0f;
		}
	}
	void SetPlaySpeed( float speed )
	{
		playSpeed = speed;
	}
	int GetFrameIndex() const
	{
		return curIdx;
	}
	void PlayGDI(HDC hdc, const Vec2<T>& topLeft, float power, COLORREF chroma )
	{
		static Image::ImageGDI sprite( spritePath );
		const auto curSprite = frames[curIdx].sprite;
		const auto curPivot = frames[curIdx].pivot;
		const Vec2<T> curTopLeft = topLeft -curPivot;

		if ( curIdx == 1 )
		{
			int a = 9;
		}

		const Vec2<T>& size = { curSprite.GetWidth(), curSprite.GetHeight() };
		surf.DrawImageChromaGDI( hdc, sprite.GetHBitmap(), curTopLeft, ( size * (T)power ), curSprite.GetTopLeft(), size, chroma );
	}
	void PlayGDIPlus(Gdiplus::Graphics& gfx, const Vec2<T>& topLeft, const Vec2<T>& size, const Gdiplus::Color& chroma )
	{
		static Image::ImageGDIPlus sprite( spritePath );
		const auto curSprite = frames[curIdx].sprite;
		surf.DrawImageChromaPlus( gfx, sprite, topLeft, size, curSprite.GetTopLeft(), { curSprite.GetWidth(), curSprite.GetHeight() }, 0, chroma );
	}

private:
	Surface<T> surf;
	SpriteType spriteType;
	const std::wstring spritePath;
	std::vector<Frame> frames;
	float playSpeed;
	float playTime = 0.0f;
	int curIdx = 0;
};