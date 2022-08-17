#pragma once

#include "framework.h"

#include <memory>
#include "ActorTag.h"
#include "Vec2.h"
#include "Collider.h"
#include "CollisionManager.h"

class Actor
{
public:
	Actor( ActorTag tag, const Vec2<float>& startPos, std::unique_ptr<Collider<float>> pCollider )
		:
		tag( tag ),
		pos( startPos ),
		pCollider( std::move( pCollider ) )
	{}
	virtual ~Actor() {}
	Actor( const Actor& ) = default;
	Actor( Actor&& ) = default;
	Actor& operator=( const Actor& ) = default;
	Actor& operator=( Actor&& ) = default;

	virtual void Update( float dt, class Scene& scene ) = 0;
	virtual void Draw( HDC hdc ) = 0;

	void DrawCollider( Gdiplus::Graphics& gfx )
	{
		pCollider->Draw( gfx, { 144,255,255,255 } );
	}
	bool IsCollideWith( const CollisionManager<float>& cm, const Collider<float>& otherCollider ) const
	{
		return cm.IsOverlapWithAABB( *pCollider, otherCollider );
	}
	void SetDestroy()
	{
		shouldDestroy = true;
	}
	bool ShouldDestroy() const
	{
		return shouldDestroy;
	}
	bool IsTagSameWith( ActorTag tag_in ) const
	{
		return tag == tag_in;
	}

protected:
	Vec2<float> GetPos() const
	{
		return pos;
	}
	void SetPos(const Vec2<float>& pos_in)
	{
		pos = pos_in;
		pCollider->SetPos( pos_in );
	}

protected:
	// Actor State
	ActorTag tag;
	bool shouldDestroy = false;
	bool isStatic = false;

private:
	Vec2<float> pos;
	std::unique_ptr<Collider<float>> pCollider;
};