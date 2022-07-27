#pragma once

#include <memory>
#include <string>

#include "framework.h"
#include "ImageCodex.h"
#include "Vec2.h"

namespace Image
{
	template <typename T = int>
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
		Vec2<T> GetImageSize() const
		{
			return { (T)bitmapImage.bmWidth, (T)bitmapImage.bmHeight };
		}
	private:
		HBITMAP hImage;
		BITMAP bitmapImage;
	};

	template <typename T = int>
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
		Vec2<T> GetImageSize() const
		{
			return { (T)pImage->GetWidth(), (T)pImage->GetHeight() };
		}
	private:
		std::shared_ptr<Gdiplus::Image> pImage;
	};
}