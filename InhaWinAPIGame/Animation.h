#pragma once

#include <vector>
#include "Surface.h"
#include "Image.h"
#include "UtilSH.h"
#include <fstream>

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
	{
	}

	void SetTransform( const Mat3<float>& transform )
	{
		surf.SetTransformation( transform );
	}
	void Update( float dt, float playSpeed )
	{
		playTime += dt;
		if ( playTime >= playSpeed )
		{
			curIdx = UtilSH::GetSafeIndex( frames, curIdx + 1 );
			playTime = 0.0f;
		}
	}
	int GetFrameIndex() const
	{
		return curIdx;
	}
	std::vector<Frame> GetFrames() const
	{
		return frames;
	}
	void PlayGDI(HDC hdc, const Image::ImageGDI<T>& image, const Vec2<T>& topLeft, float power, COLORREF chroma )
	{
		const auto curSprite = frames[curIdx].sprite;
		const auto curPivot = frames[curIdx].pivot * (T)power;
		const Vec2<T> curTopLeft = topLeft -curPivot;

		if ( curIdx == 1 )
		{
			int a = 9;
		}

		const Vec2<T>& size = { curSprite.GetWidth(), curSprite.GetHeight() };
		surf.DrawImageChromaGDI( hdc, image.GetHBitmap(), curTopLeft, ( size * (T)power ), curSprite.GetTopLeft(), size, chroma );
	}

	void LoadFramesFromFile( const std::wstring& filePath )
	{
		std::ifstream in( filePath, std::ios::binary );

		if ( in.is_open() )
		{
			if ( !frames.empty() )
			{
				frames.clear();
			}

			while ( !in.eof() )
			{
				Frame frame;
				in.read( reinterpret_cast<char*>(&frame), sizeof( Frame ) );
				frames.push_back( frame );
			}
		}
		in.close();
	}

	void SaveFramesToFile(const std::wstring& filePath)
	{
		std::ofstream out( filePath, std::ios::binary );

		if ( out.is_open() )
		{
			
			for ( Frame& frame : frames )
			{
				out.write( reinterpret_cast<char*>(&frame), sizeof( Frame ) );
			}
			
		}
		out.close();
	}

private:
	Surface<T> surf;
	SpriteType spriteType;
	std::vector<Frame> frames;
	float playTime = 0.0f;
	int curIdx = 0;
};