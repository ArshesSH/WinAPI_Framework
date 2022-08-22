#pragma once

#include <vector>
#include "Surface.h"
#include "Image.h"
#include "UtilSH.h"
#include "FileManager.h"
#include <iostream>

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
	Animation() {};
	Animation( SpriteType spriteType, const std::vector<Frame>& frames )
		:
		spriteType(spriteType),
		frames(frames)
	{}
	Animation( SpriteType spriteType, const std::wstring& fileName )
	{
		FileManager fm( fileName, FileManager::Mode::Read );
		if ( fm.CanLoad() )
		{
			fm.LoadVector( frames );
		}
	}

	void SetTransform( const Mat3<float>& transform )
	{
		surf.SetTransformation( transform );
	}

	void Update( float dt, float playSpeed )
	{
		if ( !isStop )
		{
			playTime += dt;
			if ( playTime >= playSpeed )
			{
				curIdx = UtilSH::GetSafeIndex( frames, curIdx + 1 );
				playTime = 0.0f;
			}
		}
	}

	int GetFrameIndex() const
	{
		return curIdx;
	}

	bool IsEnd() const
	{
		return curIdx == frames.size() - 1;
	}

	bool IsStopped() const
	{
		return isStop;
	}

	void SetStop(bool stop = true)
	{
		isStop = stop;
	}


	std::vector<Frame> GetFrames() const
	{
		return frames;
	}
	void PlayGDI(HDC hdc, const Image::ImageGDI<T>& image, const Vec2<T>& topLeft, float power, COLORREF chroma )
	{
		const auto curSprite = frames[curIdx].sprite;
		const auto curPivot = frames[curIdx].pivot;
		const Vec2<T> curTopLeft = (topLeft - curPivot) * (T)power;

		const Vec2<T>& size = { curSprite.GetWidth(), curSprite.GetHeight() };
		surf.DrawImageChromaGDI( hdc, image.GetHBitmap(), curTopLeft, ( size * (T)power ), curSprite.GetTopLeft(), size, chroma );
	}
	void PlayByCamGDI( HDC hdc, const Image::ImageGDI<T>& image, const Vec2<T>& topLeft, float power, COLORREF chroma, bool isFlipped = false )
	{
		_Rect<T> curSprite = frames[curIdx].sprite;

		const auto curPivotX = isFlipped ? curSprite.GetWidth() -frames[curIdx].pivot.x : frames[curIdx].pivot.x;

		auto curPivot = (Vec2<T>{ curPivotX, curSprite.GetHeight() - frames[curIdx].pivot.y })* (T)power;

		const Vec2<T> curTopLeft = (topLeft - curPivot);
		if ( isFlipped )
		{
			const auto transform = Mat3<T>::Translation( { (T)(image.GetImageSize().x), (T)0 } ) * Mat3<T>::ScaleIndependent( -1, 1 );
			const auto tl = curSprite.GetTopLeft();
			curSprite = _Rect<T>{ transform * curSprite.GetTopLeft(), transform * curSprite.GetBottomRight() };
			curPivot = transform * curPivot;
		}

		const Vec2<T>& size = { curSprite.GetWidth(), curSprite.GetHeight() };

		surf.DrawImageChromaGDI( hdc, image.GetHBitmap(), curTopLeft, (size * (T)power), curSprite.GetTopLeft(), size, chroma );

	}
	void PlayByCamGDIFlip( HDC hdc, const Image::ImageGDI<T>& image, const Vec2<T>& topLeft, float power, COLORREF chroma )
	{
		const auto curSprite = frames[curIdx].sprite;
		const auto curPivot = (Vec2<T>{ curSprite.GetWidth() - frames[curIdx].pivot.x, curSprite.GetHeight() - frames[curIdx].pivot.y })* (T)power;
		const Vec2<T> curTopLeft = Mat3<T>::ScaleIndependent( -1, 1 ) *
			Mat3<T>::Translation( { (T)(image.GetImageSize().x / 2), (T)0 } ) * (topLeft - curPivot);
		const Vec2<T>& size = { curSprite.GetWidth(), curSprite.GetHeight() };

		surf.DrawImageChromaGDI( hdc, image.GetHBitmap(), curTopLeft, (size * (T)power), curSprite.GetTopLeft(), size, chroma );
	}

	void SetFrameIndex( int idx )
	{
		curIdx = idx;
	}

private:
	Surface<T> surf;
	SpriteType spriteType;
	std::vector<Frame> frames;
	float playTime = 0.0f;
	int curIdx = 0;
	bool isStop = false;
};