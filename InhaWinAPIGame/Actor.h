#pragma once

#include <string>
#include "ImageCodex.h"
#include "Surface.h"

class Actor
{
public:
	Actor() {}
	virtual ~Actor() {}
	Actor( const Actor& ) = default;
	Actor( Actor&& ) = default;
	Actor& operator=( const Actor& ) = default;
	Actor& operator=( Actor&& ) noexcept = default;
	virtual void Draw( Gdiplus::Graphics& gfx ) = 0;
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
};