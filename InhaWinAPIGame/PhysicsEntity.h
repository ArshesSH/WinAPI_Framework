#pragma once

#include "GeometricObject.h"
#include "Circle.h"
#include "Rect.h"
#include "Star.h"

#include "Mat3.h"
#include <memory>
#include "MathSH.h"

class PhysicsEntity
{
public:
	enum class Type
	{
		Circle,
		Rect,
		Star
	};

	class TypeTrait
	{
	public:
		virtual ~TypeTrait() = default;
		virtual Type GetType() const = 0;
		virtual std::unique_ptr<TypeTrait> Clone() const = 0;
	};

public:
	enum class State
	{
		Normal,
		Collided
	};

public:
	PhysicsEntity( Type type, const Vec2<float>& pos, int id, float size_in, const Vec2<float>& vel, float angle_in, float spinFreq, int nFlares );
	PhysicsEntity( Type type, const Vec2<float>& pos, int id, float width, float height, const Vec2<float>& vel, float angle_in, float spinFreq, int nFlares );

	bool operator==( const PhysicsEntity& rhs ) const;
	bool operator!=( const PhysicsEntity& rhs ) const;

	void Update( float dt );
	void Draw( HDC hdc ) const;

	Vec2<float> GetCenter() const;
	float GetCenterX() const;
	float GetCenterY() const;
	float GetSize() const;
	float GetWidth() const;
	float GetHeight() const;
	Vec2<float> GetLeftTop() const;
	Vec2<float> GetRightBottom() const;
	float GetOuterRadius() const;
	float GetAngle() const;
	Type GetType() const;
	RECT GetRECT() const;
	const TypeTrait& GetEntityType() const;
	Vec2<float> GetVelocity() const;
	std::vector<Vec2<float>> GetVertices() const;
	bool WasCollided() const;
	float GetSizeForAdd() const;
	int GetFlareCount() const;
	float GetSpinFreq() const;

	void AddSize( float size );
	void SetSizeForAdd( float size );
	void SetCenter( const Vec2<float>& c );
	void SetCenterX( float x );
	void SetCenterY( float y );
	void SetAngle( float angle_in );
	void SetVelocity( const Vec2<float>& v );
	void SetState( const State& s );
	void SetStateToNormal();
	void SetStateToCollide();
	void ReboundX();
	void ReboundY();
	void ApplyTransformation( const Mat3<float>& transformation_in );
	void MovePos( float dt )
	{
		const Vec2<float> curPos = pObj->GetCenter() + (vel * dt);
		pObj->SetCenter( curPos );
	}
	void MovePos( const Vec2<float> vel_in, float dt )
	{
		const Vec2<float> curPos = pObj->GetCenter() + (vel_in * dt);
		pObj->SetCenter( curPos );
	}

private:
	std::unique_ptr<GeometricObject<float>> pObj;
	std::unique_ptr<TypeTrait> pType;
	Vec2<float> vel;
	Mat3<float> transform = Mat3<float>::Identity();
	State objState = State::Normal;
	float sizeForAdd = 0.0f;
	float angle = 0.0f;
	float spinFreq = 0.0f;
	float time = 0.0f;
	float collideTime = 0.0f;
	int id;
};