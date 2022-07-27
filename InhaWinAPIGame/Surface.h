#pragma once

#include "framework.h"
#include <ObjIdl.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")

#include <string>
#include <vector>
#include "Vec2.h"
#include "Mat3.h"
#include "Rect.h"
#include "Image.h"
#include "CoordSH.h"

template<typename T>
class Surface
{
public:
	void ApplyTransformation( const Mat3<float>& transformation_in )
	{
		transform = transformation_in;
	}

	// String
	void DrawStringGDI(HDC hdc, T x, T y, const std::wstring& str )
	{
		TextOut( hdc, (int)x, (int)y, str.c_str(), str.size() );
	}
	void DrawStringGDI( HDC hdc, const Vec2<T>& pos, const std::wstring& str )
	{
		TextOut( hdc, (int)pos.x, (int)pos.y, str.c_str(), str.size() );
	}
	void DrawStringPlus( Gdiplus::Graphics& graphics, const std::wstring& str, const Vec2<T>& pos, Gdiplus::Color color, float fontSize = 24.0f,
		const std::wstring& fontType = L"Consolas", const Gdiplus::FontStyle& fontStyle = Gdiplus::FontStyleRegular )
	{
		using namespace Gdiplus;

		SolidBrush brush( color );
		FontFamily fontFamily( fontType.c_str() );
		Font font( &fontFamily, fontSize, fontStyle, UnitPixel );
		PointF pointF( pos.x, pos.y );

		graphics.DrawString( str.c_str(), str.size(), &font, pointF, &brush );
	}

	// Rect
	void DrawRectGDI(HDC hdc, T left, T top, T right, T bottom, COLORREF color )
	{
		HBRUSH hBrush;
		HBRUSH oldBrush;
		hBrush = CreateSolidBrush( color );
		oldBrush = (HBRUSH)SelectObject( hdc, hBrush );

		auto topLeft = transform * Vec3<float>{ left, top, 1 };
		auto bottomRight = transform * Vec3<float>{ right, bottom, 1 };

		Rectangle( hdc, (int)topLeft.x, (int)topLeft.y, (int)bottomRight.x, (int)bottomRight.y );
		SelectObject( hdc, oldBrush );
		DeleteObject( hBrush );
	}
	void DrawRectGDI( HDC hdc, const _Rect<T>& rect, COLORREF color )
	{
		DrawRectGDI( hdc, (int)rect.left, (int)rect.top, (int)rect.right, (int)rect.bottom, color );
	}
	void DrawRectPlus( Gdiplus::Graphics& graphics, const Vec2<T>& topLeft, const Vec2<T>& bottomRight, const Gdiplus::Color color, float penWidth )
	{
		using namespace Gdiplus;

		const auto tl = transform * Vec2<float>( topLeft );
		const auto br = transform * Vec2<float>( bottomRight );
		const float width = br.x - tl.x;
		const float height = br.y - tl.y;

		Pen pen( color, penWidth );
		graphics.DrawRectangle( &pen, tl.x, tl.y, width, height );
	}
	template <typename R>
	void DrawRectPlus( Gdiplus::Graphics& graphics, R rect, const Gdiplus::Color& color, float penWidth )
	{
		DrawRectPlus( graphics, { rect.X, rect.Y }, { rect.X + rect.Width, rect.Y + rect.Height }, color, penWidth );
	}
	void DrawRectPlus( Gdiplus::Graphics& graphics, const Vec2<T>& topLeft, T width, T height, Gdiplus::Color color, float penWidth )
	{
		DrawRectPlus( graphics, topLeft, { topLeft.x + width, topLeft.y + height }, color, penWidth );
	}
	void DrawRectPlus( Gdiplus::Graphics& graphics, const _Rect<T>& rect, Gdiplus::Color color, float penWidth )
	{
		DrawRectPlus( graphics, { rect.left, rect.top }, { rect.right, rect.bottom }, color, penWidth );
	}

	// Arc
	void DrawArcPlus( Gdiplus::Graphics& graphics, const Vec2<T>& topLeft, const Vec2<T>& bottomRight,
		Gdiplus::Color color, float penWidth, float startAngle, float sweepAngle )
	{
		using namespace Gdiplus;

		const auto tl = transform * Vec2<float>( topLeft );
		const auto br = transform * Vec2<float>( bottomRight );
		const float width = br.x - tl.x;
		const float height = br.y - tl.y;

		Gdiplus::RectF r( { tl.x, tl.y }, {width, height} );
		DrawArc( graphics, color, penWidth, r, startAngle, sweepAngle );
	}
	template <typename R>
	void DrawArcPlus( Gdiplus::Graphics& graphics, R rect, Gdiplus::Color color, float penWidth, float startAngle, float sweepAngle )
	{
		DrawArcPlus( graphics, { rect.X, rect.Y }, { rect.X + rect.Width, rect.Y + rect.Height }, color, penWidth, startAngle, sweepAngle );
	}
	void DrawArcPlus( Gdiplus::Graphics& graphics, Gdiplus::Color color, float penWidth,
		const Vec2<T>& topLeft, T width, T height, float startAngle, float sweepAngle )
	{
		DrawArcPlus( graphics, topLeft, { topLeft.x + width, topLeft.y + height }, color, penWidth, startAngle, sweepAngle );
	}

	// Fill Rect
	void DrawFillRectPlus( Gdiplus::Graphics& graphics, const Vec2<T>& topLeft, const Vec2<T>& bottomRight, Gdiplus::Color color )
	{
		using namespace Gdiplus;

		const auto tl = transform * Vec2<float>( topLeft );
		const auto br = transform * Vec2<float>( bottomRight );
		const float width = br.x - tl.x;
		const float height = br.y - tl.y;
		Gdiplus::RectF r( { tl.x, tl.y }, { width, height } );

		SolidBrush brush( color );
		graphics.FillRectangle( &brush, r );
	}
	template <typename R>
	void DrawFillRectPlus( Gdiplus::Graphics& graphics, R rect, Gdiplus::Color color )
	{
		DrawFillRectPlus( graphics, { rect.X, rect.Y }, { rect.X + rect.Width, rect.Y + rect.Height }, color );
	}
	void DrawFillRectPlus( Gdiplus::Graphics& graphics, const Vec2<T>& topLeft, T width, T height, Gdiplus::Color color )
	{
		DrawFillRectPlus( graphics, topLeft, { topLeft.x + width, topLeft.y + height }, color );
	}

	// Draw Line
	void DrawLinePlus( Gdiplus::Graphics& graphics, Gdiplus::Color color, float penWidth,
		const Vec2<T>& v1, const Vec2<T>& v2 )
	{
		using namespace Gdiplus;
		Pen pen( color, penWidth );
		
		// DO THIS

		graphics.DrawLine( &pen, p1, p2 );
	}

	void DrawLinePlus( Gdiplus::Graphics& graphics, Gdiplus::Color color, float penWidth,
		const Gdiplus::Point& v1, const Gdiplus::Point& v2 )
	{
		using namespace Gdiplus;
		Pen pen( color, penWidth );
		graphics.DrawLine( &pen, v1, v2 );
	}

	template <typename P>
	void DrawPolygonPlus( Gdiplus::Graphics& graphics, Gdiplus::Color color, float penWidth, const P& points, int pointCnt )
	{
		using namespace Gdiplus;

		Pen pen( color, penWidth );
		graphics.DrawPolygon( &pen, &points, pointCnt );
	}
	template <typename P>
	void DrawFillPolygonPlus( Gdiplus::Graphics& graphics, Gdiplus::Color color, const P& points, int pointCnt )
	{
		using namespace Gdiplus;

		SolidBrush brush( color );
		graphics.FillPolygon( &brush, &points, pointCnt );
	}

	void DrawImageNonChromaGDI(HDC hdc, const HBITMAP& hBitmap,  const Vec2<T>& topLeft, const Vec2<T>& bottomRight,
		const Vec2<T>& imageStart, const Vec2<T>& imageEnd )
	{
		HDC hMemDC = CreateCompatibleDC( hdc );
		HBITMAP hOldBitmap = (HBITMAP)SelectObject( hMemDC, hBitmap );
		StretchBlt( hdc, (int)topLeft.x, (int)topLeft.y, (int)bottomRight.x, (int)bottomRight.y,
			hMemDC, (int)imageStart.x, (int)imageStart.y, (int)imageEnd.x, (int)imageEnd.y, SRCCOPY );
		SelectObject( hMemDC, hOldBitmap );
		DeleteObject( hMemDC );
	}
	void DrawImageNonChromaGDI( HDC hdc, const Image::ImageGDI<T>& image, const Vec2<T>& topLeft, const Vec2<T>& bottomRight,
		const Vec2<T>& imageStart, const Vec2<T>& imageEnd )
	{
		DrawImageNonChromaGDI( hdc, image.GetHBitmap(), topLeft, bottomRight, imageStart, imageEnd );
	}
	void DrawImageNonChromaPlus( Gdiplus::Graphics& graphics, Gdiplus::Image* image, const Vec2<T>& topLeft, const Vec2<T>& bottomRight,
		const Vec2<T>& imageStart, const Vec2<T>& imageEnd )
	{
		using namespace Gdiplus;

		const Gdiplus::Rect r( (int)topLeft.x, (int)topLeft.y, (int)(bottomRight.x - topLeft.x), (int)(bottomRight.y - topLeft.y) );
		graphics.DrawImage( image, r, (int)imageStart.x, (int)imageStart.y, (int)imageEnd.x, (int)imageEnd.y, UnitPixel );
	}
	void DrawImageNonChromaPlus( Gdiplus::Graphics& graphics, const Image::ImageGDIPlus<T>& image, const Vec2<T>& topLeft, const Vec2<T>& bottomRight,
		const Vec2<T>& imageStart, const Vec2<T>& imageEnd )
	{
		DrawImageNonChromaPlus( graphics, image.GetImagePtr(), topLeft, bottomRight, imageStart, imageEnd );
	}
	void DrawRotateImageNonChromaPlus( Gdiplus::Graphics& graphics, Gdiplus::Image* image,
		const Vec2<T>& topLeft, const Vec2<T>& bottomRight,
		const Vec2<T>& imageStart, const Vec2<T>& imageEnd, const Vec2<T>& rotateCenter, float angle )
	{
		using namespace Gdiplus;

		Matrix mat;
		mat.RotateAt( angle, { rotateCenter.x, rotateCenter.y } );
		graphics.SetTransform( &mat );

		const Gdiplus::Rect r( (int)topLeft.x, (int)topLeft.y, (int)(bottomRight.x - topLeft.x), (int)(bottomRight.y - topLeft.y) );
		graphics.DrawImage( image, r, (int)imageStart.x, (int)imageStart.y, (int)imageEnd.x, (int)imageEnd.y, UnitPixel );

		mat.Reset();
		graphics.SetTransform( &mat );
	}
	void DrawRotateImageNonChromaPlus( Gdiplus::Graphics& graphics, const Image::ImageGDIPlus<T>& image,
		const Vec2<T>& topLeft, const Vec2<T>& bottomRight,
		const Vec2<T>& imageStart, const Vec2<T>& imageEnd, const Vec2<T>& rotateCenter, float angle )
	{
		DrawRotateImageNonChromaPlus( graphics, image.GetImagePtr(), topLeft, bottomRight, imageStart, imageEnd, rotateCenter, angle );
	}

	void DrawImageChromaGDI( HDC hdc, const HBITMAP& hBitmap, const Vec2<T>& topLeft, const Vec2<T>& bottomRight,
		const Vec2<T>& imageStart, const Vec2<T>& imageEnd, COLORREF chroma = RGB(255,0,255) )
	{
		HDC hMemDC = CreateCompatibleDC( hdc );
		HBITMAP hOldBitmap = (HBITMAP)SelectObject( hMemDC, hBitmap );
		StretchBlt( hdc, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, hMemDC, imageStart.x, imageStart.y, imageEnd.x, imageEnd.y, SRCCOPY );
		TransparentBlt( hdc, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, hMemDC,
			imageStart.x, imageStart.y, imageEnd.x, imageEnd.y, chroma );
		SelectObject( hMemDC, hOldBitmap );
		DeleteObject( hMemDC );
	}
	void DrawImageChromaGDI( HDC hdc, const Image::ImageGDI<T>& image, const Vec2<T>& topLeft, const Vec2<T>& bottomRight,
		const Vec2<T>& imageStart, const Vec2<T>& imageEnd, COLORREF chroma = RGB( 255, 0, 255 ) )
	{
		DrawImageChromaGDI( hdc, image.GetHBitmap(), topLeft, bottomRight, imageStart, imageEnd, chroma );
	}
	void DrawImageChromaPlus( Gdiplus::Graphics& graphics, Gdiplus::Image* image, const Vec2<T>& topLeft, const Vec2<T>& bottomRight,
		const Vec2<T>& imageStart, const Vec2<T>& imageEnd, int angle = 0,
		Gdiplus::Color lowChroma = { 245, 0, 245 }, Gdiplus::Color highChroma = { 255,10,255 } )
	{
		using namespace Gdiplus;

		ImageAttributes imgAttr;
		imgAttr.SetColorKey( lowChroma, highChroma );

		const Gdiplus::Rect r( (int)topLeft.x, (int)topLeft.y, (int)(bottomRight.x - topLeft.x), (int)(bottomRight.y - topLeft.y) );
		graphics.DrawImage( image, r, imageStart.x, imageStart.y, imageEnd.x, imageEnd.y, UnitPixel, &imgAttr );
	}
	void DrawImageChromaPlus( Gdiplus::Graphics& graphics, const Image::ImageGDIPlus<T>& image, const Vec2<T>& topLeft, const Vec2<T>& bottomRight,
		const Vec2<T>& imageStart, const Vec2<T>& imageEnd, int angle = 0,
		Gdiplus::Color lowChroma = { 245, 0, 245 }, Gdiplus::Color highChroma = { 255,10,255 } )
	{
		DrawImageChromaPlus( graphics, image.GetImagePtr(), topLeft, bottomRight, imageStart, imageEnd, angle, lowChroma, highChroma );
	}
	void DrawRotateImageChromaPlus( Gdiplus::Graphics& graphics, Gdiplus::Image* image, const Vec2<T>& topLeft, const Vec2<T>& bottomRight,
		const Vec2<T>& imageStart, const Vec2<T>& imageEnd, const Vec2<T>& rotateCenter, float angle,
		Gdiplus::Color lowChroma = { 245, 0, 245 }, Gdiplus::Color highChroma = { 255,10,255 } )
	{
		using namespace Gdiplus;

		Matrix mat;
		mat.RotateAt( angle, { rotateCenter.x, rotateCenter.y } );
		graphics.SetTransform( &mat );

		ImageAttributes imgAttr;
		imgAttr.SetColorKey( lowChroma, highChroma );

		const Gdiplus::Rect r( (int)topLeft.x, (int)topLeft.y, (int)(bottomRight.x - topLeft.x), (int)(bottomRight.y - topLeft.y) );
		graphics.DrawImage( image, r, imageStart.x, imageStart.y, imageEnd.x, imageEnd.y, UnitPixel, &imgAttr );

		mat.Reset();
		graphics.SetTransform( &mat );
	}

	void DrawRotateImageChromaPlus( Gdiplus::Graphics& graphics, const Image::ImageGDIPlus<T>& image, const Vec2<T>& topLeft, const Vec2<T>& bottomRight,
		const Vec2<T>& imageStart, const Vec2<T>& imageEnd, const Vec2<T>& rotateCenter, float angle,
		Gdiplus::Color lowChroma = { 245, 0, 245 }, Gdiplus::Color highChroma = { 255,10,255 } )
	{
		DrawRotateImageChromaPlus( graphics, image.GetImagePtr(), topLeft, bottomRight, imageStart, imageEnd, rotateCenter, angle, lowChroma, highChroma );
	}

private:
	Mat3<float> transform = Mat3<float>::Identity();
};

