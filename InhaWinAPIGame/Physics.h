#pragma once

template<typename T>
class Physics
{
public:
	Physics()
		:
		pos( { (T)0, (T)0 } ),
		dir( { (T)0, (T)0 } ),
		linearSpeed( (T)0 ),
		angularSpeed( (T)0 )
	{}
	Physics( const Vec2& pos, const Vec2& vel, T angularSpeed )
		:
		pos( pos ),
		vel( vel ),
		angularSpeed( angularSpeed )
	{}

	void MovePos( T dt )
	{
		pos += vel * dt;
	}

private:
	Vec2<T> pos;
	Vec2<T> vel;
	T angularSpeed;
};