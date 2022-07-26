#include "DynamicActor.h"

DynamicActor::DynamicActor( const std::wstring imageName, const Vec2<float>& pos, const Vec2<float>& vel, float width, float height, int id )
	:
	pImage( ImageCodex::Retrieve( imageName ) ),
	rigidBody( PhysicsEntity::Type::Rect, pos, id, width, height, vel, 0.0f, 0.0f, 0 )
{}

bool DynamicActor::isOverlapWith( RECT other )
{
	const Vec2<float> curPos = rigidBody.GetCenter();
	const float width = rigidBody.GetWidth() * 0.5f;
	const float height = rigidBody.GetHeight() * 0.5f;

	return curPos.x + width > other.left && curPos.x - width < other.right
		&& curPos.y + height > other.top && curPos.y - height < other.bottom;
}

inline void DynamicActor::AddPos( const Vec2<float> addPos )
{
	rigidBody.SetCenter( rigidBody.GetCenter() + addPos );
}
