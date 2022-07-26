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

template<typename T>
class Surface
{
public:
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

	void DrawRectGDI(HDC hdc, T left, T top, T right, T bottom, COLORREF color )
	{
		HBRUSH hBrush;
		HBRUSH oldBrush;
		hBrush = CreateSolidBrush( color );
		oldBrush = (HBRUSH)SelectObject( hdc, hBrush );
		Rectangle( hdc, (int)left, (int)top, (int)right, (int)bottom );
		SelectObject( hdc, oldBrush );
		DeleteObject( hBrush );
	}
	void DrawRectGDI( HDC hdc, const _Rect<T>& rect, COLORREF color )
	{
		HBRUSH hBrush;
		HBRUSH oldBrush;
		hBrush = CreateSolidBrush( color );
		oldBrush = (HBRUSH)SelectObject( hdc, hBrush );
		Rectangle( hdc, (int)rect.left, (int)rect.top, (int)rect.right, (int)rect.bottom );
		SelectObject( hdc, oldBrush );
		DeleteObject( hBrush );
	}


	template <typename R>
	void DrawRectPlus( Gdiplus::Graphics& graphics, Gdiplus::Color color, float penWidth, R rect )
	{
		using namespace Gdiplus;

		Pen pen( color, penWidth );
		graphics.DrawRectangle( &pen, rect );
	}
	void DrawRectPlus( Gdiplus::Graphics& graphics, Gdiplus::Color color, float penWidth, const Vec2<T>& topLeft, T width, T height )
	{
		using namespace Gdiplus;

		Gdiplus::Rect r( { (int)topLeft.x, (int)topLeft.y }, { (int)width, (int)height } );
		DrawRectPlus( graphics, color, penWidth, r );
	}
	void DrawRectPlus( Gdiplus::Graphics& graphics, Gdiplus::Color color, float penWidth, _Rect<T> rect )
	{
		using namespace Gdiplus;

		Gdiplus::Rect r( { (int)rect.left, (int)rect.top }, { (int)rect.GetWidth(), (int)rect.GetHeight() } );
		DrawRectPlus( graphics, color, penWidth, r );
	}

	template <typename R>
	void DrawArcPlus( Gdiplus::Graphics& graphics, Gdiplus::Color color, float penWidth, R rect, float startAngle, float sweepAngle )
	{
		using namespace Gdiplus;

		Pen pen( color, penWidth );
		graphics.DrawArc( &pen, rect, startAngle, sweepAngle );
	}
	void DrawArcPlus( Gdiplus::Graphics& graphics, Gdiplus::Color color, float penWidth,
		const Vec2<T>& topLeft, T width, T height, float startAngle, float sweepAngle )
	{
		using namespace Gdiplus;

		Gdiplus::Rect r( { (int)topLeft.x, (int)topLeft.y }, { (int)width, (int)height } );
		DrawArc( graphics, color, penWidth, r, startAngle, sweepAngle );
	}

	template <typename R>
	void DrawFillRectPlus( Gdiplus::Graphics& graphics, Gdiplus::Color color, R rect )
	{
		using namespace Gdiplus;

		SolidBrush brush( color );
		graphics.FillRectangle( &brush, rect );
	}
	void DrawFillRectPlus( Gdiplus::Graphics& graphics, Gdiplus::Color color, const Vec2<T>& topLeft, T width, T height )
	{
		using namespace Gdiplus;

		Gdiplus::RectF r( { topLeft.x, topLeft.y }, { width, height } );
		DrawFillRectPlus( graphics, color, r );

		//SolidBrush brush( color );
		//Gdiplus::RectF r( { topLeft.x, topLeft.y }, { width, height } );
		//graphics.FillRectangle( &brush, r );
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

	void DrawImageNonChromaPlus( Gdiplus::Graphics& graphics, Gdiplus::Image* image, const Vec2<T>& topLeft, const Vec2<T>& bottomRight,
		const Vec2<T>& imageStart, const Vec2<T>& imageEnd )
	{
		using namespace Gdiplus;

		const Gdiplus::Rect r( (int)topLeft.x, (int)topLeft.y, (int)(bottomRight.x - topLeft.x), (int)(bottomRight.y - topLeft.y) );
		graphics.DrawImage( image, r, (int)imageStart.x, (int)imageStart.y, (int)imageEnd.x, (int)imageEnd.y, UnitPixel );
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
		graphics.DrawImage( image, r, imageStart.x, imageStart.y, imageEnd.x, imageEnd.y, UnitPixel );

		mat.Reset();
		graphics.SetTransform( &mat );
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

private:
	Mat3<float> transform = Mat3<float>::Identity();
};

