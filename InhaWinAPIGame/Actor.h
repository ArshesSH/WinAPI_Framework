#pragma once

#include "framework.h"
#include "ActorTag.h"

class Actor
{
public:
	Actor(ActorTag tag)
		:
		tag(tag)
	{}
	virtual ~Actor() {}
	Actor( const Actor& ) = default;
	Actor( Actor&& ) = default;
	Actor& operator=( const Actor& ) = default;
	Actor& operator=( Actor&& ) noexcept = default;
	virtual void Update( float dt, class Game& game ) = 0;
	virtual void Draw( HDC hdc ) = 0;
	void SetDestroy()
	{
		shouldDestroy = true;
	}
	bool ShouldDestroy() const
	{
		return shouldDestroy;
	}
protected:
	bool shouldDestroy = false;
	ActorTag tag;
};