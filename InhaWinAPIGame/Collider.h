#pragma once

#include "Vec2.h"
#include "Rect.h"
#include <vector>

template <typename T>
class Collider
{

public:
	Collider(const Vec2<T>& pos)
		:
		pos(pos)
	{}
	
	void UpdatePos( const Vec2<T>& pos_in )
	{
		pos = pos_in;
	}

	virtual bool IsOverlapWith( const Collider& other ) const = 0;

private:

protected:
	Vec2<T> pos;
};


template <typename T>
class ConvexCollider : Collider<T>
{
public:
	ConvexCollider(const Vec2<T>& pos, const _Rect<T>& rect)
		:
		Collider(pos)
	{
		vertices.reserve( 4 );
		vertices.emplace_back( rect.left, rect.top );
		vertices.emplace_back( rect.right, rect.top );
		vertices.emplace_back( rect.right, rect.bottom );
		vertices.emplace_back( rect.left, rect.bottom );
	}
	ConvexCollider( const Vec2<T>& pos, const std::vector<Vec2<T>>& vertices )
		:
		Collider( pos ),
		vertices( vertices )
	{}

	bool IsOverlapWith( const Collider& other ) const override
	{
	}

private:
	std::vector<Vec2<T>> vertices;
};