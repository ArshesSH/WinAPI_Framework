#include "SigmaHead.h"

#include "Scene.h"

void SigmaHead::Update( float dt, Scene& scene )
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
#endif // !NDEBUG
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

	}
}
