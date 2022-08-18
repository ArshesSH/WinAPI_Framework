#include "PlayerX.h"

#include "Scene.h"
#include "PlayerXBehavior.h"
#include "PlayerXIdle.h"
#include "PlayerXWalk.h"


PlayerX::PlayerX( const Vec2<float>& pivotPos, const Vec2<float>& colliderRelativePos )
	:
	Character( ActorTag::Player, pivotPos, RectF::FromCenter( colliderRelativePos, colliderHalfWidth, colliderHalfHeight ), defaultSpeed,
		L"Images/RockmanX5/X/ForthArmorSprite.bmp", L"Images/RockmanX5/X/ForthArmorSpriteFlip.bmp" ),
	pivotGizmo( Vec2<int>( pivotPos ) )
{
	animationMap[(int)State::Idle] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Idle.anim" );
	animationMap[(int)State::IdleBlink] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/IdleBlink.anim" );
	animationMap[(int)State::WalkStart] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/WalkStart.anim" );
	animationMap[(int)State::WalkLoop] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/walkLoop.anim" );

	curAnimation = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Idle.anim" );
	pBehavior = std::make_unique<PlayerXIdle>( *this );
}

void PlayerX::Update( float dt, Scene& scene )
{
	KbdInput(dt, scene);
	pBehavior->Do( dt, scene );
	curAnimation.Update( dt, animSpeed );
	
	// Debug
	const auto pos = GetPos();
	const auto colliderPos = GetColliderPos();
	imgPosStr = L"imgPos: (" + std::to_wstring( pos.x ) + L", " + std::to_wstring( pos.y ) + L")";
	colliderPosStr = L"colliderPos : (" + std::to_wstring( colliderPos.x ) + L", " + std::to_wstring( colliderPos.y ) + L")";

	pivotGizmo.SetPos( Vec2<int>( pos ) );
	pivotGizmo.SetTransform( scene.AccessCamera().GetTransform() );

	if ( state != prevState )
	{
		SetAnimation( state, animSpeed );
		prevState = state;
	}

}

void PlayerX::Draw( HDC hdc )
{
	if ( !isFlipped )
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

void PlayerX::KbdInput( float dt, Scene& scene )
{
	state = State::Idle;
	dir = { 0.0f, 0.0f };
	if ( GetAsyncKeyState( VK_LEFT ) & 0x8001 )
	{
		dir = dirLeft;
		Move( dt, scene );
		SetFlip( false );
	}
	if ( GetAsyncKeyState( VK_RIGHT ) & 0x8001 )
	{
		dir = dirRight;
		Move( dt, scene );
		SetFlip();
	}
}

