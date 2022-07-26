#pragma once

#include "Actor.h"

#include <string>
#include "ImageCodex.h"
#include "PhysicsEntity.h"

class DynamicActor : public Actor
{
public:
	DynamicActor( const std::wstring imageName, const Vec2<float>& pos, const Vec2<float>& vel, float width, float height, int id );
	virtual ~DynamicActor() {}
	DynamicActor( const DynamicActor& ) = default;
	DynamicActor( DynamicActor&& ) = default;
	DynamicActor& operator=( const DynamicActor& ) = default;
	DynamicActor& operator=( DynamicActor&& ) noexcept = default;

	virtual void Update( float dt, class Scene& scene ) = 0;
	virtual void Draw( Gdiplus::Graphics& gfx ) = 0;
	virtual bool isOverlapWith( RECT other );
	virtual void AddPos( const Vec2<float> addPos );

protected:
	std::shared_ptr<Gdiplus::Image> pImage;
	Vec2<int> imageEnd = { pImage->GetWidth(), pImage->GetHeight() };
	PhysicsEntity rigidBody;
};