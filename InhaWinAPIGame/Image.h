#pragma once

#include <memory>
#include <string>

#include "framework.h"
#include "ImageCodex.h"
#include "Vec2.h"

namespace Image
{
	class ImageGDI
	{
	public:
		ImageGDI( const std::wstring& imageName )
			:
			hImage( (HBITMAP)LoadImage( NULL, imageName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION ) )
		{
			GetObject( hImage, sizeof( BITMAP ), &bitmapImage );
		}
		~ImageGDI()
		{
			DeleteObject( hImage );
		}
		HBITMAP GetHBitmap() const
		{
			return hImage;
		}
		BITMAP GetBitmap() const
		{
			return bitmapImage;
		}
		Vec2<int> GetImageSize() const
		{
			return { bitmapImage.bmWidth, bitmapImage.bmHeight };
		}
	private:
		HBITMAP hImage;
		BITMAP bitmapImage;
	};

	class ImageGDIPlus
	{
	public:
		ImageGDIPlus( const std::wstring imageName )
			:
			pImage( ImageCodex::Retrieve( imageName ) )
		{}
		Gdiplus::Image* GetImagePtr() const
		{
			return pImage.get();
		}
	private:
		std::shared_ptr<Gdiplus::Image> pImage;
	};
}