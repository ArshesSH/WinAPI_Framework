#pragma once

#include "framework.h"
#include <ObjIdl.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
#include <string>
#include <memory>
#include "Vec2.h"
#include "GeometricObject.h"

namespace Surface
{
	void DrawString(Gdiplus::Graphics& graphics, const std::wstring& str, const Vec2<float>& pos, Gdiplus::Color color, float fontSize = 24.0f,
		const std::wstring& fontType = L"Consolas", const Gdiplus::FontStyle& fontStyle = Gdiplus::FontStyleRegular );

	void DrawRect(Gdiplus::Graphics& graphics, Gdiplus::Color color, float penWidth, const Vec2<float>& topLeft, float width, float height );
	void DrawRect(Gdiplus::Graphics& graphics, Gdiplus::Color color, float penWidth, Gdiplus::RectF rectF );

	void DrawArc(Gdiplus::Graphics& graphics, Gdiplus::Color color, float penWidth, const Vec2<float>& topLeft, float width, float height, float startAngle, float sweepAngle );
	void DrawArc(Gdiplus::Graphics& graphics, Gdiplus::Color color, float penWidth, Gdiplus::RectF rectF, float startAngle, float sweepAngle );

	void DrawFillRect(Gdiplus::Graphics& graphics, Gdiplus::Color color, const Vec2<float>& topLeft, float width, float height );
	void DrawFillRect(Gdiplus::Graphics& graphics, Gdiplus::Color color, Gdiplus::RectF rectF );

	void DrawLine( Gdiplus::Graphics& graphics, Gdiplus::Color color, float penWidth,
		const Gdiplus::Point& v1, const Gdiplus::Point& v2 );
	void DrawLines( Gdiplus::Graphics& graphics, Gdiplus::Color color, float penWidth,
		const std::vector<Gdiplus::Point>& vertices );

	void DrawPolygon(Gdiplus::Graphics& graphics, Gdiplus::Color color, float penWidth, const Gdiplus::PointF& points, int pointCnt );
	void DrawPolygon( Gdiplus::Graphics& graphics, Gdiplus::Color color, float penWidth, const Gdiplus::Point& points, int pointCnt );
	void DrawFillPolygon(Gdiplus::Graphics& graphics, Gdiplus::Color color, const Gdiplus::PointF& points, int pointCnt );
	void DrawFillPolygon( Gdiplus::Graphics& graphics, Gdiplus::Color color, const Gdiplus::Point& points, int pointCnt );

	void ClipingPath( Gdiplus::Graphics& graphics, const Gdiplus::GraphicsPath& path );

	void DrawImageNonChroma(Gdiplus::Graphics& graphics, Gdiplus::Image* image, const Vec2<float>& topLeft, const Vec2<float>& bottomRight,
		const Vec2<int>& imageStart, const Vec2<int>& imageEnd );

	void DrawImageNonChroma( Gdiplus::Graphics& graphics, Gdiplus::Image* image, const Vec2<int>& topLeft, const Vec2<int>& bottomRight,
		const Vec2<int>& imageStart, const Vec2<int>& imageEnd );

	void DrawImageNonChroma(Gdiplus::Graphics& graphics, Gdiplus::Image* image, const Vec2<float>& topLeft, const Vec2<float>& bottomRight,
		const Vec2<int>& imageStart, const Vec2<int>& imageEnd, const Vec2<float>& rotateCenter, float angle );

	void DrawImageChroma(Gdiplus::Graphics& graphics, Gdiplus::Image* image, const Vec2<float>& topLeft, const Vec2<float>& bottomRight,
		const Vec2<int>& imageStart, const Vec2<int>& imageEnd, int angle = 0,
		Gdiplus::Color lowChroma = { 245, 0, 245 }, Gdiplus::Color highChroma = { 255,10,255 } );

	void DrawImageChroma( Gdiplus::Graphics& graphics, Gdiplus::Image* image, const Vec2<int>& topLeft, const Vec2<int>& bottomRight,
		const Vec2<int>& imageStart, const Vec2<int>& imageEnd, int angle = 0,
		Gdiplus::Color lowChroma = { 245, 0, 245 }, Gdiplus::Color highChroma = { 255,10,255 } );

	void DrawImageChroma(Gdiplus::Graphics& graphics, Gdiplus::Image* image, const Vec2<float>& topLeft, const Vec2<float>& bottomRight,
		const Vec2<int>& imageStart, const Vec2<int>& imageEnd, const Vec2<float>& rotateCenter, float angle,
		Gdiplus::Color lowChroma = { 245, 0, 245 }, Gdiplus::Color highChroma = { 255,10,255 } );
};

