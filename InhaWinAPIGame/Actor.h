#pragma once

#include "framework.h"

#include <memory>
#include "ActorTag.h"
#include "Vec2.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "Mat3.h"
#include <algorithm>
#include <iterator>

class Actor
{
protected:
	template <class T>
	class Behavior
	{
	public:
		virtual ~Behavior() {}
		virtual Behavior* Update( T& Actor, class Scene& scene, float dt ) = 0;
		virtual void Activate( T& Actor, class Scene& scene ) {}
		void SetSuccessorStates( std::vector<Behavior*> successors )
		{
			statePtrStack = std::move( successors );
		}
		void PushSucessorState( Behavior* successor )
		{
			statePtrStack.push_back( successor );
		}
		void PushSucessorStates( std::vector<Behavior*> successors )
		{
			std::copy(
				successors.begin(), successors.end(), std::back_inserter( statePtrStack )
			);
		}
		bool HasSucessors() const
		{
			return !statePtrStack.empty();
		}
		Behavior* PassTorch()
		{
			auto ps = statePtrStack.back();
			statePtrStack.pop_back();
			ps->SetSuccessorStates( std::move( statePtrStack ) );
			return ps;
		}
	private:
		std::vector<Behavior*> statePtrStack;
	};

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
	virtual void SetTransform( const Mat3<float>& transform ) = 0;
	virtual void Draw( HDC hdc ) = 0;
	virtual void ApplyDamage( int damage ) = 0;

	void DrawCollider( Gdiplus::Graphics& gfx, const Mat3<float>& transform )
	{
		pCollider->UpdateMatrix( transform );
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
	const std::unique_ptr<Collider<float>>& GetColliderPtr() const
	{
		return pCollider;
	}

	Vec2<float> GetPos() const
	{
		return pos;
	}
	Vec2<float> GetColliderPos() const
	{
		return pCollider->GetCenter();
	}

protected:
	void SetPos(const Vec2<float>& pos_in)
	{
		const Vec2<float> relativePos = pCollider->GetCenter() - pos;
		pos = pos_in;
		pCollider->SetPosByCenter( pos_in + relativePos );
	}
	void ChangeColliderSize( float width, float height )
	{
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