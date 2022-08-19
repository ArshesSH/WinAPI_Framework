#include "PlayerX.h"

#include "Scene.h"
#include "PlayerXBehavior.h"
#include "PlayerXIdle.h"
#include "PlayerXWalk.h"

#include "BvPlayerXIdle.h"
#include "BvPlayerXWalk.h"


PlayerX::PlayerX( const Vec2<float>& pivotPos, const Vec2<float>& colliderRelativePos )
	:
	Character( ActorTag::Player, pivotPos, RectF::FromCenter( colliderRelativePos, colliderHalfWidth, colliderHalfHeight ), defaultSpeed,
		L"Images/RockmanX5/X/ForthArmorSprite.bmp", L"Images/RockmanX5/X/ForthArmorSpriteFlip.bmp" ),
	pivotGizmo( Vec2<int>( pivotPos ) ),
	pBehavior(std::make_unique<Idle>())
{
	animationMap[(int)AnimationState::Idle] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Idle.anim" );
	animationMap[(int)AnimationState::IdleBlink] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/IdleBlink.anim" );
	animationMap[(int)AnimationState::WalkStart] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/WalkStart.anim" );
	animationMap[(int)AnimationState::WalkLoop] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/walkLoop.anim" );

	curAnimation = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Idle.anim" );
	//pBehavior = std::make_unique<PlayerXIdle>( *this );
}

//PlayerX::~PlayerX()
//{
//	//if ( pBehavior )
//	//{
//	//	delete pBehavior;
//	//}
//}

void PlayerX::Update( float dt, Scene& scene )
{
	KbdInput(dt, scene);
	//pBehavior->Do( dt, scene );

	while ( auto pNewState = pBehavior->Update(*this, scene, dt) )
	{
		//delete pBehavior;
		pBehavior.reset( pNewState );
		pBehavior->Activate( *this, scene );
	}

	curAnimation.Update( dt, animSpeed );
	

	// Debug
	const auto pos = GetPos();
	const auto colliderPos = GetColliderPos();
	imgPosStr = L"imgPos: (" + std::to_wstring( pos.x ) + L", " + std::to_wstring( pos.y ) + L")";
	colliderPosStr = L"colliderPos : (" + std::to_wstring( colliderPos.x ) + L", " + std::to_wstring( colliderPos.y ) + L")";

	pivotGizmo.SetPos( Vec2<int>( pos ) );
	pivotGizmo.SetTransform( scene.AccessCamera().GetTransform() );

}

void PlayerX::Draw( HDC hdc )
{
	if ( !isFacingRight )
	{
		curAnimation.PlayByCamGDI( hdc, sprite, Vec2<int>( GetPos() ), 2, chroma );
	}
	else
	{
		curAnimation.PlayByCamGDI( hdc, spriteFlipped, Vec2<int>( GetPos() ), 2, chroma, true );
	}


	// Debug
	Surface<int> surf;
	
	surf.DrawStringGDI( hdc, { 0,0 }, imgPosStr );
	surf.DrawStringGDI( hdc, { 0,20 }, colliderPosStr );
	
	pivotGizmo.Draw( hdc );
}

void PlayerX::Walk(float dt, Scene& scene )
{
	if ( curState != State::Walk )
	{
		curState = State::Walk;
	}

	Move( dt, scene );
	SetFacingRight( false );
	pBehavior->SetSuccessorStates( { new WalkStart, new WalkLoop } );
}

void PlayerX::KbdInput( float dt, Scene& scene )
{
	dir = { 0.0f, 0.0f };
	curState = State::Idle;

	if ( GetAsyncKeyState( VK_LEFT ) & 0x8001 )
	{
		dir = dirLeft;
		Walk( dt, scene );
	}
	if ( GetAsyncKeyState( VK_RIGHT ) & 0x8001 )
	{
		dir = dirRight;
		isFacingRight = true;
		Walk( dt, scene );
	}

	if ( GetAsyncKeyState( 'X' ) & 0x8001 )
	{

	}


}

