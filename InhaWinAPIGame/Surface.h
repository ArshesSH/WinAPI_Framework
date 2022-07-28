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
	void SetTransformation( const Mat3<float>& transformation_in )
	{
		transform = transformation_in;
	}
	void ApplyTransformation( const Mat3<float>& transformation_in )
	{
		transform = transformation_in * transform;
	}
	const Mat3<float>& GetTransformation() const
	{
		return transform;
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
	void DrawStringPlus( Gdiplus::Graphics& graphics, const std::wstring& str, const Vec2<T>& pos, const Gdiplus::Color& color, float fontSize = 24.0f,
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

		const auto topLeft =  transform * Vec3<float>{ left, top, 1 };
		const auto bottomRight = transform * Vec3<float>{ right, bottom, 1 };

		Rectangle( hdc, (int)topLeft.x, (int)topLeft.y, (int)bottomRight.x, (int)bottomRight.y );
		SelectObject( hdc, oldBrush );
		DeleteObject( hBrush );
	}
	void DrawRectGDI( HDC hdc, const _Rect<T>& rect, COLORREF color )
	{
		DrawRectGDI( hdc, (int)rect.left, (int)rect.top, (int)rect.right, (int)rect.bottom, color );
	}
	void DrawRectPlus( Gdiplus::Graphics& graphics, const Vec2<T>& topLeft, const Vec2<T>& bottomRight, const Gdiplus::Color& color, float penWidth )
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
	void DrawRectPlus( Gdiplus::Graphics& graphics, const Vec2<T>& topLeft, T width, T height, const Gdiplus::Color& color, float penWidth )
	{
		DrawRectPlus( graphics, topLeft, { topLeft.x + width, topLeft.y + height }, color, penWidth );
	}
	void DrawRectPlus( Gdiplus::Graphics& graphics, const _Rect<T>& rect, const Gdiplus::Color& color, float penWidth )
	{
		DrawRectPlus( graphics, { rect.left, rect.top }, { rect.right, rect.bottom }, color, penWidth );
	}

	// Arc
	void DrawArcPlus( Gdiplus::Graphics& graphics, const Vec2<T>& topLeft, const Vec2<T>& bottomRight,
		const Gdiplus::Color& color, float penWidth, float startAngle, float sweepAngle )
	{
		using namespace Gdiplus;

		const auto tl = transform * Vec2<float>( topLeft );
		const auto br = transform * Vec2<float>( bottomRight );
		const float width = br.x - tl.x;
		const float height = br.y - tl.y;

		const Gdiplus::RectF r( { tl.x, tl.y }, {width, height} );
		DrawArc( graphics, color, penWidth, r, startAngle, sweepAngle );
	}
	template <typename R>
	void DrawArcPlus( Gdiplus::Graphics& graphics, R rect, const Gdiplus::Color& color, float penWidth, float startAngle, float sweepAngle )
	{
		DrawArcPlus( graphics, { rect.X, rect.Y }, { rect.X + rect.Width, rect.Y + rect.Height }, color, penWidth, startAngle, sweepAngle );
	}
	void DrawArcPlus( Gdiplus::Graphics& graphics, const Gdiplus::Color& color, float penWidth,
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
		const Gdiplus::RectF r( { tl.x, tl.y }, { width, height } );

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
	void DrawLinePlus( Gdiplus::Graphics& graphics, const Vec2<T>& v1, const Vec2<T>& v2,  const Gdiplus::Color& color, float penWidth)
	{
		using namespace Gdiplus;
		Pen pen( color, penWidth );
		
		CoordSH::PointConversion<float> conversion;
		const Gdiplus::PointF p1 = conversion( (transform * v1) );
		const Gdiplus::PointF p2 = conversion( (transform * v2) );

		graphics.DrawLine( &pen, p1, p2 );
	}
	void DrawLinePlus( Gdiplus::Graphics& graphics, const Gdiplus::Point& p1, const Gdiplus::Point& p2, const Gdiplus::Color& color, float penWidth)
	{
		CoordSH::PointConversion<float> conversion;
		DrawLinePlus( graphics, conversion( p1 ), conversion( p2 ), color, penWidth );
	}

	// DrawPolygon
	void DrawPolygonPlus( Gdiplus::Graphics& graphics, const Gdiplus::Color& color, float penWidth, const std::vector<Vec2<T>>& vertices, int pointCnt )
	{
		using namespace Gdiplus;

		std::vector<Gdiplus::PointF> points;
		points.reserve( vertices.size() );

		CoordSH::PointConversion<float> conversion;
		for ( auto& v : vertices )
		{
			points.push_back( conversion( transform * v ) );
		}

		Pen pen( color, penWidth );
		graphics.DrawPolygon( &pen, &points, pointCnt );
	}
	void DrawPolygonPlus( Gdiplus::Graphics& graphics, const Gdiplus::Color& color, float penWidth, const std::vector<Gdiplus::PointF>& points, int pointCnt )
	{
		using namespace Gdiplus;

		std::vector<Gdiplus::PointF> tPoints;
		tPoints.reserve( points.size() );
		CoordSH::PointConversion<float> conversion;
		for ( auto& p : points )
		{
			points.push_back( conversion( transform * conversion(p) ) );
		}

		Pen pen( color, penWidth );
		graphics.DrawPolygon( &pen, &tPoints, pointCnt );
	}

	// Fill Polygon
	void DrawFillPolygonPlus( Gdiplus::Graphics& graphics, const Gdiplus::Color& color, const std::vector<Vec2<T>>& vertices, int pointCnt )
	{
		using namespace Gdiplus;

		std::vector<Gdiplus::PointF> points;
		points.reserve( vertices.size() );

		CoordSH::PointConversion<float> conversion;
		for ( auto& v : vertices )
		{
			points.push_back( conversion( transform * v ) );
		}

		SolidBrush brush( color );
		graphics.FillPolygon( &brush, &points, pointCnt );
	}
	void DrawFillPolygonPlus( Gdiplus::Graphics& graphics, const Gdiplus::Color& color, const std::vector<Gdiplus::PointF>& points, int pointCnt )
	{
		using namespace Gdiplus;

		std::vector<Gdiplus::PointF> tPoints;
		tPoints.reserve( points.size() );
		CoordSH::PointConversion<float> conversion;
		for ( auto& p : points )
		{
			points.push_back( conversion( transform * conversion( p ) ) );
		}

		SolidBrush brush( color );
		graphics.FillPolygon( &brush, &tPoints, pointCnt );
	}

	// Draw NonChroma Image
	void DrawImageNonChromaGDI(HDC hdc, const HBITMAP& hBitmap, const Vec2<T>& topLeft, const Vec2<T>& bottomRight,
		const Vec2<T>& imageStart, const Vec2<T>& imageEnd )
	{
		HDC hMemDC = CreateCompatibleDC( hdc );
		HBITMAP hOldBitmap = (HBITMAP)SelectObject( hMemDC, hBitmap );

		const auto tl = transform * Vec2<float>( topLeft );
		const auto br = transform * Vec2<float>( bottomRight );

		StretchBlt( hdc, (int)tl.x, (int)tl.y, (int)br.x, (int)br.y,
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

		const auto tl = transform * Vec2<float>( topLeft );
		const auto br = transform * Vec2<float>( bottomRight );
		const float width = br.x - tl.x;
		const float height = br.y - tl.y;
		const Gdiplus::RectF r( { tl.x, tl.y }, { width, height } );

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

		DrawImageNonChromaPlus( graphics, image, topLeft, bottomRight, imageStart, imageEnd, rotateCenter, angle );

		mat.Reset();
		graphics.SetTransform( &mat );
	}
	void DrawRotateImageNonChromaPlus( Gdiplus::Graphics& graphics, const Image::ImageGDIPlus<T>& image,
		const Vec2<T>& topLeft, const Vec2<T>& bottomRight,
		const Vec2<T>& imageStart, const Vec2<T>& imageEnd, const Vec2<T>& rotateCenter, float angle )
	{
		DrawRotateImageNonChromaPlus( graphics, image.GetImagePtr(), topLeft, bottomRight, imageStart, imageEnd, rotateCenter, angle );
	}


	// Draw Chorma Image
	void DrawImageChromaGDI( HDC hdc, const HBITMAP& hBitmap, const Vec2<T>& topLeft, const Vec2<T>& bottomRight,
		const Vec2<T>& imageStart, const Vec2<T>& imageEnd, COLORREF chroma = RGB(255,0,255) )
	{
		HDC hMemDC = CreateCompatibleDC( hdc );
		HBITMAP hOldBitmap = (HBITMAP)SelectObject( hMemDC, hBitmap );

		const auto tl = transform * Vec2<float>( topLeft );
		const auto br = transform * Vec2<float>( bottomRight );

		TransparentBlt( hdc, tl.x, tl.y, br.x, br.y, hMemDC,
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
		const Gdiplus::Color& lowChroma = { 245, 0, 245 }, const Gdiplus::Color& highChroma = { 255,10,255 } )
	{
		using namespace Gdiplus;

		ImageAttributes imgAttr;
		imgAttr.SetColorKey( lowChroma, highChroma );

		const auto tl = transform * Vec2<float>( topLeft );
		const auto br = transform * Vec2<float>( bottomRight );
		const float width = br.x - tl.x;
		const float height = br.y - tl.y;
		const Gdiplus::RectF r( { tl.x, tl.y }, { width, height } );

		graphics.DrawImage( image, r, imageStart.x, imageStart.y, imageEnd.x, imageEnd.y, UnitPixel, &imgAttr );
	}
	void DrawImageChromaPlus( Gdiplus::Graphics& graphics, const Image::ImageGDIPlus<T>& image, const Vec2<T>& topLeft, const Vec2<T>& bottomRight,
		const Vec2<T>& imageStart, const Vec2<T>& imageEnd, int angle = 0,
		const Gdiplus::Color& lowChroma = { 245, 0, 245 }, const Gdiplus::Color& highChroma = { 255,10,255 } )
	{
		DrawImageChromaPlus( graphics, image.GetImagePtr(), topLeft, bottomRight, imageStart, imageEnd, angle, lowChroma, highChroma );
	}
	void DrawRotateImageChromaPlus( Gdiplus::Graphics& graphics, Gdiplus::Image* image, const Vec2<T>& topLeft, const Vec2<T>& bottomRight,
		const Vec2<T>& imageStart, const Vec2<T>& imageEnd, const Vec2<T>& rotateCenter, float angle,
		const Gdiplus::Color& lowChroma = { 245, 0, 245 }, const Gdiplus::Color& highChroma = { 255,10,255 } )
	{
		using namespace Gdiplus;

		Matrix mat;
		mat.RotateAt( angle, { rotateCenter.x, rotateCenter.y } );
		graphics.SetTransform( &mat );

		DrawImageChromaPlus( graphics, image, topLeft, bottomRight, imageStart, imageEnd, rotateCenter, angle, lowChroma, highChroma );

		mat.Reset();
		graphics.SetTransform( &mat );
	}

	void DrawRotateImageChromaPlus( Gdiplus::Graphics& graphics, const Image::ImageGDIPlus<T>& image, const Vec2<T>& topLeft, const Vec2<T>& bottomRight,
		const Vec2<T>& imageStart, const Vec2<T>& imageEnd, const Vec2<T>& rotateCenter, float angle,
		const Gdiplus::Color& lowChroma = { 245, 0, 245 }, const Gdiplus::Color& highChroma = { 255,10,255 } )
	{
		DrawRotateImageChromaPlus( graphics, image.GetImagePtr(), topLeft, bottomRight, imageStart, imageEnd, rotateCenter, angle, lowChroma, highChroma );
	}

private:
	Mat3<float> transform = Mat3<float>::Identity();
};

