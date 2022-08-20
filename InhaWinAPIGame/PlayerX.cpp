#include "PlayerX.h"

#include "Scene.h"

#include "BvPlayerXIdle.h"
#include "BvPlayerXWalk.h"
#include "BvPlayerXDash.h"
#include "BvPlayerXAirbone.h"



PlayerX::PlayerX( const Vec2<float>& pivotPos, const Vec2<float>& colliderRelativePos )
	:
	Character( ActorTag::Player, pivotPos, RectF::FromCenter( colliderRelativePos, colliderHalfWidth, colliderHalfHeight ), 200.0f,
		L"Images/RockmanX5/X/ForthArmorSprite.bmp", L"Images/RockmanX5/X/ForthArmorSpriteFlip.bmp" ),
	pivotGizmo( Vec2<int>( pivotPos ) ),
	pBehavior( std::make_unique<Idle>() ),
	gravity( 9.8f )
{
	animationMap[(int)AnimationState::Idle] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Idle.anim" );
	animationMap[(int)AnimationState::IdleBlink] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/IdleBlink.anim" );
	animationMap[(int)AnimationState::WalkStart] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/WalkStart.anim" );
	animationMap[(int)AnimationState::WalkLoop] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/WalkLoop.anim" );
	animationMap[(int)AnimationState::DashStart] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/DashStart.anim" );
	animationMap[(int)AnimationState::DashLoop] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/DashLoop.anim" );
	animationMap[(int)AnimationState::DashEnd] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/DashEnd.anim" );
	animationMap[(int)AnimationState::Airbone] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Airbone.anim" );

	curAnimation = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Idle.anim" );
}

void PlayerX::Update( float dt, Scene& scene )
{
	KbdInput();
	curAnimation.Update( dt, animPlaySpeed );
	UpdatePlayerState();
	UpdatePlayerBehavior();
	isOnGround = IsCollideWithWall( this->GetNextPos( dt ), scene );

	// Update Behavior
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

void PlayerX::UpdatePlayerState()
{
	if ( this->vel.x > 0.0f )
	{
		isFacingRight = true;
	}
	else
	{
		isFacingRight = false;
	}

	if ( isOnGround )
	{
		if ( isXKeyDown )
		{
			moveState = MoveState::Jump;
		}
		else if ( isRightKeyDown ^ isLeftKeyDown )
		{
			if ( isZKeyDown && !isDashEnd )
			{
				moveState = MoveState::Dash;
			}
			else
			{
				moveState = MoveState::Walk;
			}
		}
		else if ( isZKeyDown && !isDashEnd )
		{
			moveState = MoveState::Dash;
		}
		else
		{
			moveState = MoveState::Idle;
		}
	}
	else
	{
		moveState = MoveState::Airbone;
	}
}

void PlayerX::UpdatePlayerBehavior()
{
	if ( CheckMoveStateChange() || CheckAttackStateChange() )
	{
		switch ( attackState )
		{
		case PlayerX::AttackState::NoAttack:
			{
				switch ( moveState )
				{
				case PlayerX::MoveState::Idle:
					{
						oldMoveState = moveState;
						pBehavior->PushSucessorState( new Idle );
					}
					break;
				case PlayerX::MoveState::Walk:
					{
						oldMoveState = moveState;
						pBehavior->PushSucessorState( new Walk );
					}
					break;
				case PlayerX::MoveState::Dash:
					{
						oldMoveState = moveState;
						pBehavior->PushSucessorState( new Dash );
					}
					break;
				case PlayerX::MoveState::Jump:
					break;
				case PlayerX::MoveState::Airbone:
					{
						oldMoveState = moveState;
						pBehavior->PushSucessorState( new Airbone );
					}
					break;
				case PlayerX::MoveState::Hover:
					break;
				case PlayerX::MoveState::Land:
					break;
				case PlayerX::MoveState::Ladder:
					break;
				case PlayerX::MoveState::Wall:
					break;
				default:
					break;
				}
			}
			break;
		case PlayerX::AttackState::Charge:
			break;
		case PlayerX::AttackState::Shoot:
			break;
		case PlayerX::AttackState::Hurt:
			break;
		default:
			break;
		}
	}
}

void PlayerX::KbdInput()
{
	if ( GetAsyncKeyState( VK_LEFT ) & 0x8001 )
	{
		isLeftKeyDown = true;
	}
	else
	{
		isLeftKeyDown = false;
	}

	if ( GetAsyncKeyState( VK_RIGHT ) & 0x8001 )
	{
		isRightKeyDown = true;
	}
	else 
	{
		isRightKeyDown = false;
	}

	if ( GetAsyncKeyState( 'Z' ) & 0x8001 )
	{
		isZKeyDown = true;
	}
	else
	{
		isDashEnd = false;
		isZKeyDown = false;
	}

	if ( GetAsyncKeyState( 'X' ) & 0x8001 )
	{
		isXKeyDown = true;
	}
	else
	{
		isXKeyDown = false;
	}


}

