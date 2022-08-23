#include "SigmaHead.h"

#include "BvSigmaHeadDecide.h"
#include "Scene.h"

SigmaHead::SigmaHead( const Vec2<float>& pos, std::mt19937& rng )
	:
	Enemy( pos, { 0.0f, 0.0f }, colliderHalfWidth, colliderHalfHeight, moveSpeed,
		L"Images/RockmanX5/Sigma/SigmaHead.bmp", L"Images/RockmanX5/Sigma/SigmaHead.bmp" ),
	hitCollider( RectF::FromCenter( pos, hitColX, hitColY ) )
{
	animationMap[(int)AnimationState::Idle] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/Sigma/SigmaIdle.anim" );
	animationMap[(int)AnimationState::Ready] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/Sigma/SigmaReady.anim" );
	animationMap[(int)AnimationState::Fade] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/Sigma/SigmaFade.anim" );
	animationMap[(int)AnimationState::Open] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/Sigma/SigmaOpen.anim" );
	animationMap[(int)AnimationState::Damaged] = Animation<int>( Animation<int>::SpriteType::GDI, L"Images/RockmanX5/Sigma/SigmaDamaged.anim" );

	SetAnimation( AnimationState::Idle, animPlaySpeed );
	SetInvincible();
	pBehavior = std::make_unique<Decide>( rng );
}

void SigmaHead::Update( float dt, Scene& scene )
{
	if ( IsDead() )
	{
		SetDestroy();
	}
	else
	{
		hitCollider.SetPosByCenter( GetPos() + hitColRelPos );

		curAnimation.Update( dt, animPlaySpeed );

		// Update Behavior
		while ( auto pNewState = pBehavior->Update( *this, scene, dt ) )
		{
			pBehavior.reset( pNewState );
			pBehavior->Activate( *this, scene );
		}


#ifndef NDEBUG
		hitCollider.UpdateMatrix( scene.AccessCamera().GetTransform() );
		std::cout << "hp: " << hp << std::endl;
#endif // !NDEBUG
	}
}

void SigmaHead::Draw( HDC hdc )
{
	curAnimation.PlayByCamGDI( hdc, sprite, Vec2<int>( GetPos() ), 2, chroma );

#ifndef NDEBUG
	Gdiplus::Graphics gfx( hdc );
	hitCollider.Draw( gfx, { 255,0,255,0 } );
#endif // !NDEBUG

}

void SigmaHead::ApplyDamage( int damage )
{
	if ( !isInvincible )
	{
		hp -= damage;
		SetInvincible( true );
		isDamaged = true;
	}
}

void SigmaHead::Move( float dt, const Scene& scene )
{
	const auto moveAmount = vel * dt;
	const Vec2<float> nextPos = GetPos() + moveAmount;

	SetPos( nextPos );
}
