#include "PlayerArea.h"

#include <cassert>
#include "Scene.h"

PlayerArea::PlayerArea(const std::vector<Vec2<int>>& vertices)
	:
	polygon(vertices)
{
}

PlayerArea::PlayerArea( const Gdiplus::Rect& rect )
{
	const int left = rect.GetLeft();
	const int top = rect.GetTop();
	const int right = rect.GetRight();
	const int bottom = rect.GetBottom();

	polygon.push_back( left, top );
	polygon.push_back( right, top );
	polygon.push_back( right, bottom );
	polygon.push_back( left, bottom );

	drawVertices = ArshesSH::Polygon::ConvertToVectorOfPoint( polygon );
}

void PlayerArea::Update( float dt, Scene& scene )
{
	MoveToRelativeCoord( (Vec2<int>)scene.GetSceneTopLeft() );
}

void PlayerArea::Draw( Gdiplus::Graphics& gfx )
{
	Surface::DrawFillPolygon( gfx, color, drawVertices[0], (int)drawVertices.size() );
}

void PlayerArea::MoveToRelativeCoord( const Vec2<int>& amount )
{
	drawVertices = ArshesSH::Polygon::ConvertToVectorOfPoint(polygon + amount);

}