#include "PlayerX.h"

#include "Scene.h"

#include "BvPlayerXIdle.h"
#include "BvPlayerXWalk.h"
#include "BvPlayerXDash.h"
#include "BvPlayerXAirbone.h"
#include "BvPlayerXJump.h"
#include "BvPlayerXHover.h"
#include "BvPlayerXWallSlide.h"
#include "BvPlayerXCrouch.h"
#include "BvPlayerXShoot.h"

#include "PlayerXBullet.h"

PlayerX::PlayerX( const Vec2<float>& pivotPos, const Vec2<float>& colliderRelativePos )
	:
	Character( ActorTag::Player, pivotPos, RectF::FromCenter( colliderRelativePos, colliderHalfWidth, colliderHalfHeight ), 200.0f,
		L"Images/RockmanX5/X/ForthArmorSprite.bmp", L"Images/RockmanX5/X/ForthArmorSpriteFlip.bmp" ),
	pivotGizmo( Vec2<int>( pivotPos ) ),
	pBehavior( std::make_unique<Idle>() ),
	gravity( 20.0f ),
	wallSearcher( pivotPos + Vec2<float>{-wallsearcherLength, 0.0f}, pivotPos + Vec2<float>{wallsearcherLength, 0.0f} )
{
	animationMap[(int)AnimationState::Idle] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Idle.anim" );
	animationMap[(int)AnimationState::IdleBlink] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/IdleBlink.anim" );
	animationMap[(int)AnimationState::WalkStart] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/WalkStart.anim" );
	animationMap[(int)AnimationState::WalkLoop] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/WalkLoop.anim" );
	animationMap[(int)AnimationState::DashStart] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/DashStart.anim" );
	animationMap[(int)AnimationState::DashLoop] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/DashLoop.anim" );
	animationMap[(int)AnimationState::DashEnd] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/DashEnd.anim" );
	animationMap[(int)AnimationState::Airbone] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Airbone.anim" );
	animationMap[(int)AnimationState::Jump] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Jump.anim" );
	animationMap[(int)AnimationState::Hover] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Hover.anim" );
	animationMap[(int)AnimationState::HoverFront] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/HoverFront.anim" );
	animationMap[(int)AnimationState::HoverBack] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/HoverBack.anim" );
	animationMap[(int)AnimationState::WallSlide] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/WallSlide.anim" );
	animationMap[(int)AnimationState::WallCling] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/WallCling.anim" );
	animationMap[(int)AnimationState::AirDashStart] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/AirDashStart.anim" );
	animationMap[(int)AnimationState::WallKick] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/WallKick.anim" );
	animationMap[(int)AnimationState::Crouch] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Crouch.anim" );
	animationMap[(int)AnimationState::Shoot] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Shoot.anim" );

	curAnimation = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/X/Idle.anim" );
}

void PlayerX::Update( float dt, Scene& scene )
{
	KbdInput(dt);
	//TestKbd( dt, scene );
	curAnimation.Update( dt, animPlaySpeed );

	UpdatePlayerState();
	UpdatePlayerBehavior();
	UpdateWallSearcher(dt);

	isOnWallSide = IsWallSearcherCollide( scene );

	isOnGround = IsCollideWithWall( GetColliderPos() - Vec2<float>{0, 1.0f}, scene );
	//std::cout << "isOnGround : " << std::boolalpha << isOnGround << std::endl;
	//std::cout << "Vel:{" << vel.x << ", " << vel.y << std::endl;
	//std::cout << "isOnWallSide : " << std::boolalpha << isOnWallSide << std::endl;
	
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
	wallSearcher.UpdateMatrix( scene.AccessCamera().GetTransform() );
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
	Gdiplus::Graphics gfx( hdc );
	surf.DrawStringGDI( hdc, { 0,0 }, imgPosStr );
	surf.DrawStringGDI( hdc, { 0,20 }, colliderPosStr );
	surf.DrawStringGDI( hdc, { 0,100 }, isRightKeyStr );
	surf.DrawStringGDI( hdc, { 0,120 }, isLeftKeyStr );
	DrawStateString( surf, hdc );
	DrawAnimationStateString( surf, hdc );
	pivotGizmo.Draw( hdc );

	wallSearcher.Draw( gfx, { 255,0,255,0 } );
#endif // NDEBUG
}

void PlayerX::ApplyDamage( int damage )
{
	hp -= damage;
}

void PlayerX::UpdatePlayerState()
{
	if ( !isStopFacingTrack )
	{
		if ( vel.x > 0.0f )
		{
			isFacingRight = true;
		}
		else if ( vel.x < 0.0f )
		{
			isFacingRight = false;
		}
	}


	if ( isOnGround )
	{
		hoverCount = 0;
		canAirDash = true;
		if ( isXKeyDown  && !isJumpEnd )
		{
			if ( isZKeyDown )
			{
				moveState = MoveState::DashJump;
			}
			else
			{
				moveState = MoveState::Jump;
			}
		}
		else if ( isRightKeyDown ^ isLeftKeyDown )
		{
			if ( isZKeyDown && !isDashEnd )
			{
				moveState = MoveState::Dash;
			}
			else if( !isDownKeyDown )
			{
				moveState = MoveState::Walk;
			}
		}
		else if ( isZKeyDown && !isDashEnd )
		{
			moveState = MoveState::Dash;
		}
		else if ( isDownKeyDown )
		{
			moveState = MoveState::Crouch;
		}
		else
		{
			if ( attackState == AttackState::NoAttack || chargeState == ChargeState::NoCharge )
			{
				moveState = MoveState::Idle;
			}
			if ( chargeState == ChargeState::FireNormal )
			{
				attackState = AttackState::Shoot;
			}
		}
	}
	else
	{
		if ( !isJumpNow && !isOnWallSide )
		{
			moveState = MoveState::Airbone;
		}
		if ( hoverCount == 1 )
		{
			moveState = MoveState::Hover;
		}
		if ( isZKeyDown && !isDashEnd && canAirDash )
		{
			moveState = MoveState::AirDash;
		}
		if ( !isJumpNow && isOnWallSide )
		{
			moveState = MoveState::WallSlide;
		}
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
				oldAttackState = attackState;
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
				case PlayerX::MoveState::AirDash:
					{
						oldMoveState = moveState;
						pBehavior->PushSucessorState( new AirDash );
					}
					break;
				case PlayerX::MoveState::Jump:
					{
						oldMoveState = moveState;
						pBehavior->PushSucessorState( new Jump );
					}
					break;
				case PlayerX::MoveState::DashJump:
					{
						oldMoveState = moveState;
						pBehavior->PushSucessorState( new DashJump );
					}
					break;
				case PlayerX::MoveState::Airbone:
					{
						oldMoveState = moveState;
						pBehavior->PushSucessorState( new Airbone );
					}
					break;
				case PlayerX::MoveState::Hover:
					{
						oldMoveState = moveState;
						pBehavior->PushSucessorState( new Hover );
					}
					break;
				case PlayerX::MoveState::Land:
					break;
				case PlayerX::MoveState::Ladder:
					break;
				case PlayerX::MoveState::WallSlide:
					{
						oldMoveState = moveState;
						pBehavior->PushSucessorState( new WallSlide );
					}
					break;
				case PlayerX::MoveState::WallKick:
					{
						oldMoveState = moveState;
						pBehavior->PushSucessorState( new WallKick );
					}
					break;
				case PlayerX::MoveState::Crouch:
					{
						oldMoveState = moveState;
						pBehavior->PushSucessorState( new Crouch );
					}
					break;
				default:
					break;
				}
			}
			break;
		case PlayerX::AttackState::Charge:
			break;
		case PlayerX::AttackState::Shoot:
			{
				oldAttackState = attackState;
				switch ( moveState )
				{
				case PlayerX::MoveState::Idle:
					{
						oldMoveState = moveState;
						pBehavior->PushSucessorState( new Shoot );
					}
					break;
				case PlayerX::MoveState::Walk:
					{
					}
					break;
				case PlayerX::MoveState::Dash:
					{
					}
					break;
				case PlayerX::MoveState::AirDash:
					{
					}
					break;
				case PlayerX::MoveState::Jump:
					{
					}
					break;
				case PlayerX::MoveState::DashJump:
					{
					}
					break;
				case PlayerX::MoveState::Airbone:
					{
					}
					break;
				case PlayerX::MoveState::Hover:
					{
					}
					break;
				case PlayerX::MoveState::Land:
					break;
				case PlayerX::MoveState::Ladder:
					break;
				case PlayerX::MoveState::WallSlide:
					{
					}
					break;
				case PlayerX::MoveState::WallKick:
					{
					}
					break;
				case PlayerX::MoveState::Crouch:
					{
					}
					break;
				default:
					break;
				}
			}
			break;
		case PlayerX::AttackState::Hurt:
			break;
		default:
			break;
		}
	}
}

void PlayerX::KbdInput(float dt)
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

	if ( GetAsyncKeyState( VK_DOWN ) & 0x8001 )
	{
		isDownKeyDown = true;
	}
	else
	{
		isDownKeyDown = false;
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

	if ( (GetAsyncKeyState( 'X' ) & 0x8000) && (!isXKeyInputOnce))
	{
		isXKeyInputOnce = true;
		hoverCount++;
	}
	else if( (GetAsyncKeyState( 'X' ) == 0 ))
	{
		isXKeyInputOnce = false;
	}

	if ( GetAsyncKeyState( 'X' ) & 0x8001 )
	{
		isXKeyDown = true;
	}
	else
	{
		isJumpEnd = false;
		isXKeyDown = false;
		isJumpNow = false;
	}


	if ( GetAsyncKeyState( 'C' ) & 0x8001 )
	{
		isCKeyDown = true;
		chargeTime += dt;
		if ( chargeTime >= bulletChargeMiddle )
		{
			if ( chargeTime >= bulletChargeMax )
			{
				chargeState = ChargeState::FireMax;
			}
			else
			{
				chargeState = ChargeState::FireMiddle;
			}
		}
		else
		{
			chargeState = ChargeState::FireNormal;
		}
	}
	else
	{
		chargeState = ChargeState::NoCharge;
		isCKeyDown = false;
		chargeTime = 0.0f;
	}
	std::cout << "chargeTime : " << chargeTime << std::endl;
}

void PlayerX::TestKbd( float dt, Scene& scene )
{
	vel = { 0.0f, 0.0f };
	if ( GetAsyncKeyState( VK_LEFT ) & 0x8001 )
	{
		vel.x = -300.0f;
	}
	if ( GetAsyncKeyState( VK_RIGHT ) & 0x8001 )
	{
		vel.x = 300.0f;
	}
	if ( GetAsyncKeyState( VK_UP ) & 0x8001 )
	{
		vel.y = 300.0f;
	}
	if ( GetAsyncKeyState( VK_DOWN ) & 0x8001 )
	{
		vel.y = -300.0f;
	}
	Move( dt, scene );
}

void PlayerX::UpdateWallSearcher(float dt)
{
	wallSearcher.SetPosByCenter( GetPos() + wallSearcherOffset );
}

bool PlayerX::IsWallSearcherCollide( Scene& scene )
{
	for ( const auto& pWall : scene.GetWallPtrs() )
	{
		if ( pWall->IsCollideWith( scene.GetCollisionManager(), wallSearcher ) )
		{
			return true;
		}
	}



	return false;
}

void PlayerX::SpawnBullet1( Scene& scene )
{
	const float spawnX = (isFacingRight) ? bulletSpawnDefaultX : -bulletSpawnDefaultX;
	const Vec2<float> dir = (isFacingRight) ? dirRight : dirLeft;
	const Vec2<float> realativeSpawn = { spawnX, bulletSpawnDefaultY };

	scene.AccessBulletPtrs().emplace_back(
		std::make_unique<PlayerXBullet>( PlayerXBullet::Type::Bullet1, 400.0f, dir, GetPos() + realativeSpawn, Vec2<float>{ 0.0f, 0.0f }, Vec2<float>{ bullet1Width, bullet1Height }, isFacingRight )
	);
}
