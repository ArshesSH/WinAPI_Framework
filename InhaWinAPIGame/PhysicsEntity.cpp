#include "PhysicsEntity.h"

#include "PhysicsEntityTypeTraits.h"
#include "PatternMatchingListener.h"

PhysicsEntity::PhysicsEntity( Type type, const Vec2<float>& pos, int id, float size_in, const Vec2<float>& vel, float angle_in, float spinFreq, int nFlares )
	:
	id( id ),
	vel( vel ),
	spinFreq( spinFreq )
{
	if ( type == Type::Rect )
	{
		pObj = std::make_unique<Rect<float>>( (float)pos.x, (float)pos.y, size_in, size_in );
		SetAngle( angle_in );
		pType = std::make_unique<TypeRect>();
	}
	else if ( type == Type::Circle )
	{
		pObj = std::make_unique<Circle<float>>( (float)pos.x, (float)pos.y, size_in );
		SetAngle( angle_in );
		pType = std::make_unique<TypeCircle>();
	}
	else if ( type == Type::Star )
	{
		const Vec2<float> posStar{ (float)pos.x, (float)pos.y };
		pObj = std::make_unique<Star<float>>( posStar, size_in, nFlares );
		SetAngle( angle_in );
		pType = std::make_unique<TypeStar>();
	}
}

PhysicsEntity::PhysicsEntity( Type type, const Vec2<float>& pos, int id, float width, float height, const Vec2<float>& vel, float angle_in, float spinFreq, int nFlares )
	:
	id( id ),
	vel( vel ),
	spinFreq( spinFreq )
{
	if ( type == Type::Rect )
	{
		pObj = std::make_unique<Rect<float>>( (float)pos.x, (float)pos.y, width, height );
		SetAngle( angle_in );
		pType = std::make_unique<TypeRect>();
	}
	else if ( type == Type::Circle )
	{
		pObj = std::make_unique<Circle<float>>( (float)pos.x, (float)pos.y, (std::min)(width, height) );
		SetAngle( angle_in );
		pType = std::make_unique<TypeCircle>();
	}
	else if ( type == Type::Star )
	{
		const Vec2<float> posStar{ (float)pos.x, (float)pos.y };
		pObj = std::make_unique<Star<float>>( posStar, (std::min)(width, height), nFlares );
		SetAngle( angle_in );
		pType = std::make_unique<TypeStar>();
	}
}

bool PhysicsEntity::operator==( const PhysicsEntity& rhs ) const
{
	return id == rhs.id;
}

bool PhysicsEntity::operator!=( const PhysicsEntity& rhs ) const
{
	return  id != rhs.id;
}

void PhysicsEntity::Update( float dt )
{
	{
		time += dt;
		collideTime += dt;
		MovePos( dt );
		SetAngle( spinFreq * time );
		pObj->ApplyTransformation( Mat3<float>::Rotation( angle ) );
		if ( objState == State::Collided )
		{
			if ( collideTime >= dt )
			{
				objState = State::Normal;
				collideTime = 0.0f;
			}
		}
	}
}

void PhysicsEntity::Draw( HDC hdc ) const
{
	if ( objState == State::Collided )
	{
		pObj->DrawDebug( hdc );
	}
	else
	{
		pObj->Draw( hdc );
	}
}

 Vec2<float> PhysicsEntity::GetCenter() const
{
	return pObj->GetCenter();
}

 float PhysicsEntity::GetCenterX() const
 {
	 return pObj->GetCenterX();
 }

 float PhysicsEntity::GetCenterY() const
 {
	 return pObj->GetCenterY();
 }

 float PhysicsEntity::GetSize() const
{
	return pObj->GetSize();
}

 float PhysicsEntity::GetWidth() const
 {
	 return pObj->GetWidth();
 }

 float PhysicsEntity::GetHeight() const
 {
	 return pObj->GetHeight();
 }

 Vec2<float> PhysicsEntity::GetLeftTop() const
 {
	 return pObj->GetLeftTop();
 }

 Vec2<float> PhysicsEntity::GetRightBottom() const
 {
	 return pObj->GetRightBottom();
 }

 float PhysicsEntity::GetOuterRadius() const
{
	return pObj->GetRadius();
}

 float PhysicsEntity::GetAngle() const
{
	return angle;
}

 PhysicsEntity::Type PhysicsEntity::GetType() const
 {
	 return pType->GetType();
 }

 RECT PhysicsEntity::GetRECT() const
 {
	 return pObj->GetRECT();
 }

const PhysicsEntity::TypeTrait& PhysicsEntity::GetEntityType() const
{
	return *pType;
}

Vec2<float> PhysicsEntity::GetVelocity() const
{
	return vel;
}

std::vector<Vec2<float>> PhysicsEntity::GetVertices() const
{
	return pObj->GetVertices();
}

bool PhysicsEntity::WasCollided() const
{
	return objState == State::Collided;
}

float PhysicsEntity::GetSizeForAdd() const
{
	return sizeForAdd;
}

int PhysicsEntity::GetFlareCount() const
{
	return pObj->GetFlareCount();
}

float PhysicsEntity::GetSpinFreq() const
{
	return spinFreq;
}


void PhysicsEntity::AddSize( float size )
{
	pObj->AddSize( size );
}

void PhysicsEntity::SetSizeForAdd( float size )
{
	sizeForAdd = size;
}

void PhysicsEntity::SetCenter( const Vec2<float>& c )
{
	pObj->SetCenter( c );
}

void PhysicsEntity::SetCenterX( float x )
{
	pObj->SetCenterX( x );
}

void PhysicsEntity::SetCenterY( float y )
{
	pObj->SetCenterY( y );
}

 void PhysicsEntity::SetAngle( float angle_in )
{
	angle = MathSH::WrapAngle( angle_in );
}

 void PhysicsEntity::SetVelocity( const Vec2<float>& v )
{
	vel = v;
}

 void PhysicsEntity::SetState( const State& s )
{
	objState = s;
}

 void PhysicsEntity::SetStateToNormal()
 {
	 objState = State::Normal;
 }

 void PhysicsEntity::SetStateToCollide()
 {
	 objState = State::Collided;
 }

 void PhysicsEntity::ReboundX()
 {
	 vel.x = -vel.x;
 }

 void PhysicsEntity::ReboundY()
 {
	 vel.y = -vel.y;
 }

inline void PhysicsEntity::ApplyTransformation( const Mat3<float>& transformation_in )
{
	transform = transformation_in * transform;
}