#include "PlayerX.h"

#include "Scene.h"

#include "BvPlayerXIdle.h"
#include "BvPlayerXWalk.h"
#include "BvPlayerXDash.h"



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
	animationMap[(int)AnimationState::WalkLoop] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/WalkLoop.anim" );
	animationMap[(int)AnimationState::DashStart] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/DashStart.anim" );
	animationMap[(int)AnimationState::DashLoop] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/DashLoop.anim" );

	curAnimation = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Idle.anim" );
}

void PlayerX::Update( float dt, Scene& scene )
{
	KbdInput(dt, scene);
	
	UpdateState();
	
	curAnimation.Update( dt, animSpeed );

	while ( auto pNewState = pBehavior->Update(*this, scene, dt) )
	{
		pBehavior.reset( pNewState );
		pBehavior->Activate( *this, scene );
	}

#ifndef NDEBUG
	// Debug
	const auto pos = GetPos();
	const auto colliderPos = GetColliderPos();
	imgPosStr = L"imgPos: (" + std::to_wstring( pos.x ) + L", " + std::to_wstring( pos.y ) + L")";
	colliderPosStr = L"colliderPos : (" + std::to_wstring( colliderPos.x ) + L", " + std::to_wstring( colliderPos.y ) + L")";

	const std::wstring rightStr = isRightKeyDown ? L"True" : L"False";
	const std::wstring leftStr = isLeftKeyDown ? L"True" : L"False";

	isRightKeyStr = L"Right " + rightStr ;
	isLeftKeyStr = L"Left " + leftStr;
	pivotGizmo.SetPos( Vec2<int>( pos ) );
	pivotGizmo.SetTransform( scene.AccessCamera().GetTransform() );
#endif // NDEBUG

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

#ifndef NDEBUG
	// Debug
	Surface<int> surf;
	surf.DrawStringGDI( hdc, { 0,0 }, imgPosStr );
	surf.DrawStringGDI( hdc, { 0,20 }, colliderPosStr );
	surf.DrawStringGDI( hdc, { 0,100 }, isRightKeyStr );
	surf.DrawStringGDI( hdc, { 0,120 }, isLeftKeyStr );
	DrawStateString( surf, hdc );
	DrawAnimationStateString( surf, hdc );
	pivotGizmo.Draw( hdc );
#endif // NDEBUG
}

void PlayerX::StopDash()
{
	isDash = false;
	isDashEnd = true;
	pBehavior = std::make_unique<Idle>();
	SetState( State::Idle );
}

void PlayerX::UpdateState()
{
	if ( isOnGround )
	{
		if ( isDash )
		{
			ChangeBehaviorByState( State::Dash );
			SetState( State::Dash );
		}

		else if ( (isRightKeyDown || isLeftKeyDown) && std::abs(vel.x) >= moveSpeed )
		{
			ChangeBehaviorByState( State::Walk );
			SetState( State::Walk );
		}
		else
		{
			ChangeBehaviorByState( State::Idle );
			SetState( State::Idle );
		}
	}

}

void PlayerX::ChangeBehaviorByState(State state)
{
	if ( curState != state )
	{
		curState = state;

		switch ( curState )
		{
		case PlayerX::State::Idle:
			pBehavior->PushSucessorState( new Idle );
			break;
		case PlayerX::State::Walk:
			pBehavior->PushSucessorStates( { new WalkLoop,  new WalkStart } );
			break;
		case PlayerX::State::Dash:
			pBehavior->PushSucessorState( new Dash );
			break;
		}
	}
}

void PlayerX::Walk(float dt, Scene& scene )
{
	Move( dt, scene );
}


void PlayerX::KbdInput( float dt, Scene& scene )
{
	vel = { 0.0f, 0.0f };

	if ( GetAsyncKeyState( VK_LEFT ) & 0x8001 )
	{
		vel += dirLeft * moveSpeed;

		if ( !isRightKeyDown )
		{
			isFacingRight = false;
		}
		isLeftKeyDown = true;
	}
	else
	{
		isLeftKeyDown = false;
	}

	if ( GetAsyncKeyState( VK_RIGHT ) & 0x8001 )
	{
		vel += dirRight * moveSpeed;
		if ( !isLeftKeyDown )
		{
			isFacingRight = true;
		}
		isRightKeyDown = true;
	}
	else 
	{
		isRightKeyDown = false;
	}

	if ( GetAsyncKeyState( 'Z' ) & 0x8001 )
	{
		if ( isDashEnd == false )
		{
			isDash = true;
		}
	}
	else
	{
		isDash = false;
		isDashEnd = false;
	}

	if ( GetAsyncKeyState( 'X' ) & 0x8001 )
	{

	}


}

