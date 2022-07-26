#include "Surface.h"

void Surface::DrawString( Gdiplus::Graphics& graphics, const std::wstring& str, const Vec2<float>& pos, Gdiplus::Color color, float fontSize,
	const std::wstring& fontType, const Gdiplus::FontStyle& fontStyle )
{
	using namespace Gdiplus;

	SolidBrush brush( color );
	FontFamily fontFamily( fontType.c_str() );
	Font font( &fontFamily, fontSize, fontStyle, UnitPixel );
	PointF pointF( pos.x, pos.y );

	graphics.DrawString( str.c_str(), str.size(), &font, pointF, &brush );
}

void Surface::DrawRect( Gdiplus::Graphics& graphics, Gdiplus::Color color, float penWidth, const Vec2<float>& topLeft, float width, float height)
{
	using namespace Gdiplus;

	Pen pen( color, penWidth );
	Gdiplus::RectF r( { topLeft.x, topLeft.y }, { width, height } );
	graphics.DrawRectangle( &pen, r );
}

void Surface::DrawRect( Gdiplus::Graphics& graphics, Gdiplus::Color color, float penWidth, Gdiplus::RectF rectF )
{
	using namespace Gdiplus;

	Pen pen( color, penWidth );
	graphics.DrawRectangle( &pen, rectF );
}

void Surface::DrawArc( Gdiplus::Graphics& graphics, Gdiplus::Color color, float penWidth, const Vec2<float>& topLeft, float width, float height, float startAngle, float sweepAngle )
{
	using namespace Gdiplus;

	Pen pen( color, penWidth );
	Gdiplus::RectF r( { topLeft.x, topLeft.y }, { width, height } );
	graphics.DrawArc( &pen, r, startAngle, sweepAngle );
}

void Surface::DrawArc( Gdiplus::Graphics& graphics, Gdiplus::Color color, float penWidth, Gdiplus::RectF rectF, float startAngle, float sweepAngle )
{
	using namespace Gdiplus;

	Pen pen( color, penWidth );
	graphics.DrawArc( &pen, rectF, startAngle, sweepAngle );
}

void Surface::DrawFillRect( Gdiplus::Graphics& graphics, Gdiplus::Color color, const Vec2<float>& topLeft, float width, float height )
{
	using namespace Gdiplus;

	SolidBrush brush( color );
	Gdiplus::RectF r( { topLeft.x, topLeft.y }, { width, height } );
	graphics.FillRectangle( &brush, r );
}

void Surface::DrawFillRect( Gdiplus::Graphics& graphics, Gdiplus::Color color, Gdiplus::RectF rectF )
{
	using namespace Gdiplus;

	SolidBrush brush( color );
	graphics.FillRectangle( &brush, rectF );
}

void Surface::DrawLine( Gdiplus::Graphics& graphics, Gdiplus::Color color, float penWidth,
	const Gdiplus::Point& v1, const Gdiplus::Point& v2 )
{
	using namespace Gdiplus;
	Pen pen( color, penWidth );
	graphics.DrawLine( &pen, v1, v2 );
}

void Surface::DrawLines( Gdiplus::Graphics& graphics, Gdiplus::Color color, float penWidth, const std::vector<Gdiplus::Point>& vertices )
{
	using namespace Gdiplus;
	Pen pen( color, penWidth );
	graphics.DrawLines( &pen, &vertices[0], (int)vertices.size() );
}

void Surface::DrawPolygon( Gdiplus::Graphics& graphics, Gdiplus::Color color, float penWidth, const Gdiplus::PointF& points, int pointCnt )
{
	using namespace Gdiplus;

	Pen pen( color, penWidth );
	graphics.DrawPolygon( &pen, &points, pointCnt );
}

void Surface::DrawPolygon( Gdiplus::Graphics& graphics, Gdiplus::Color color, float penWidth, const Gdiplus::Point& points, int pointCnt )
{
	using namespace Gdiplus;

	Pen pen( color, penWidth );
	graphics.DrawPolygon( &pen, &points, pointCnt );
}

void Surface::DrawFillPolygon( Gdiplus::Graphics& graphics, Gdiplus::Color color, const Gdiplus::PointF& points, int pointCnt )
{
	using namespace Gdiplus;

	SolidBrush brush( color );
	graphics.FillPolygon( &brush, &points, pointCnt );
}

void Surface::DrawFillPolygon( Gdiplus::Graphics& graphics, Gdiplus::Color color, const Gdiplus::Point& points, int pointCnt )
{
	using namespace Gdiplus;

	SolidBrush brush( color );
	graphics.FillPolygon( &brush, &points, pointCnt );
}

void Surface::ClipingPath( Gdiplus::Graphics& graphics, const Gdiplus::GraphicsPath& path )
{
	using namespace Gdiplus;
	graphics.SetClip( &path, CombineMode::CombineModeXor );

	//Pen pen( {255,255,255,255}, 1 );
	//graphics.DrawPath( &pen, &path );
}


void Surface::DrawImageNonChroma( Gdiplus::Graphics& graphics, Gdiplus::Image* image, const Vec2<float>& topLeft, const Vec2<float>& bottomRight,
	const Vec2<int>& imageStart, const Vec2<int>& imageEnd )
{
	using namespace Gdiplus;

	const Gdiplus::Rect r( (int)topLeft.x, (int)topLeft.y, (int)(bottomRight.x - topLeft.x), (int)(bottomRight.y - topLeft.y) );
	graphics.DrawImage( image, r, imageStart.x, imageStart.y, imageEnd.x, imageEnd.y, UnitPixel );
}

void Surface::DrawImageNonChroma( Gdiplus::Graphics& graphics, Gdiplus::Image* image, const Vec2<int>& topLeft, const Vec2<int>& bottomRight,
	const Vec2<int>& imageStart, const Vec2<int>& imageEnd )
{
	using namespace Gdiplus;

	const Gdiplus::Rect r( (int)topLeft.x, (int)topLeft.y, (int)(bottomRight.x - topLeft.x), (int)(bottomRight.y - topLeft.y) );
	graphics.DrawImage( image, r, imageStart.x, imageStart.y, imageEnd.x, imageEnd.y, UnitPixel );
}

void Surface::DrawImageNonChroma( Gdiplus::Graphics& graphics, Gdiplus::Image* image, const Vec2<float>& topLeft, const Vec2<float>& bottomRight,
	const Vec2<int>& imageStart, const Vec2<int>& imageEnd, const Vec2<float>& rotateCenter, float angle )
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

void Surface::DrawImageChroma( Gdiplus::Graphics& graphics, Gdiplus::Image* image, const Vec2<float>& topLeft, const Vec2<float>& bottomRight,
	const Vec2<int>& imageStart, const Vec2<int>& imageEnd, int angle, Gdiplus::Color lowChroma, Gdiplus::Color highChroma )
{
	using namespace Gdiplus;

	ImageAttributes imgAttr;
	imgAttr.SetColorKey( lowChroma, highChroma );

	const Gdiplus::Rect r( (int)topLeft.x, (int)topLeft.y, (int)(bottomRight.x - topLeft.x), (int)(bottomRight.y - topLeft.y) );
	graphics.DrawImage( image, r, imageStart.x, imageStart.y, imageEnd.x, imageEnd.y, UnitPixel, &imgAttr );
}

void Surface::DrawImageChroma( Gdiplus::Graphics& graphics, Gdiplus::Image* image, const Vec2<int>& topLeft, const Vec2<int>& bottomRight,
	const Vec2<int>& imageStart, const Vec2<int>& imageEnd, int angle, Gdiplus::Color lowChroma, Gdiplus::Color highChroma )
{
	using namespace Gdiplus;

	ImageAttributes imgAttr;
	imgAttr.SetColorKey( lowChroma, highChroma );

	const Gdiplus::Rect r( (int)topLeft.x, (int)topLeft.y, (int)(bottomRight.x - topLeft.x), (int)(bottomRight.y - topLeft.y) );
	graphics.DrawImage( image, r, imageStart.x, imageStart.y, imageEnd.x, imageEnd.y, UnitPixel, &imgAttr );
}

void Surface::DrawImageChroma( Gdiplus::Graphics& graphics, Gdiplus::Image* image, const Vec2<float>& topLeft, const Vec2<float>& bottomRight,
	const Vec2<int>& imageStart, const Vec2<int>& imageEnd, const Vec2<float>& rotateCenter, float angle, Gdiplus::Color lowChroma, Gdiplus::Color highChroma )
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
