#include "ActorTestScene.h"

#include "Game.h"
#include "PlayerX.h"

ActorTestScene::ActorTestScene( int sceneWidth, int sceneHeight, CoordinateTransformer& ct )
	:
	Scene( sceneWidth, sceneHeight, ct )
{
	actorPtrs.emplace_back( std::make_unique<PlayerX>( Vec2<float>{ 0.0f, 100.0f } ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>{-100.0f, -100.0f}, 500.0f, 50.0f ) );
}

void ActorTestScene::Update( float dt, Game & game )
{
	MoveCamera( dt );

	for ( auto& pActor : actorPtrs )
	{
		pActor->Update( dt, *this );
	}
}

void ActorTestScene::Draw( HDC hdc )
{
	Gdiplus::Graphics gfx( hdc );
	auto drawFuncs = [&]( HDC hdc, const Mat3<float>& camTransform )
	{
		for ( auto& pActor : actorPtrs )
		{
			pActor->SetTransform( camTransform );
			pActor->Draw( hdc );
			const auto& pCollider = pActor->GetColliderPtr();
			pCollider->UpdateMatrix( camTransform );
			pCollider->Draw( gfx, {144,255,255,255} );
		}

		for ( auto& pWall : wallPtrs )
		{
			const auto& pCollider = pWall->GetColliderPtr();
			pCollider->UpdateMatrix( camTransform );
			pCollider->Draw( gfx, { 144,255,0,255 } );
		}
	};

	const float screenX = (sceneBottomRight.x - sceneTopLeft.x) / 2.0f;
	const float screenY = (sceneBottomRight.y - sceneTopLeft.y) / 2.0f;

	cam.Draw( hdc, { screenX, screenY }, drawFuncs );
}


