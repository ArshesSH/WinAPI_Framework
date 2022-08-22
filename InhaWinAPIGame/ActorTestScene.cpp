#include "ActorTestScene.h"

#include "Game.h"
#include "PlayerX.h"

ActorTestScene::ActorTestScene( int sceneWidth, int sceneHeight, CoordinateTransformer& ct )
	:
	Scene( sceneWidth, sceneHeight, ct ),
	stageImage( L"Images/RockmanX5/OpeningStage.bmp" ),
	stageSize( stageImage.GetImageSize() * 2 )
{
	cam.SetPos( { 200.0f,240.0f } );
	cam.SetScale( 2.0f );
	actorPtrs.emplace_back( std::make_unique<PlayerX>( Vec2<float>{ 300.0f, 500.0f } ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>{500.0f, 30.0f}, 1000.0f, 50.0f ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>{-50.0f, 150.0f}, 50.0f, 300.0f ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>{1600.0f, 0.0f}, 250.0f, 175.0f ) );
	//groundPtrs.emplace_back( std::make_unique<Ground>( Line<float>( { -200, 200 }, { 0, 100 } ) ) );
	//groundPtrs.emplace_back( std::make_unique<Ground>( Line<float>( { -200, 150 }, { 0, 100 } ) ) );
}

void ActorTestScene::Update( float dt, Game& game )
{
	MoveCamera( dt );

	for ( const auto& pActor : actorPtrs )
	{
		pActor->Update( dt, *this );
	}
	for ( const auto& pBullet : bulletPtrs )
	{
		pBullet->Update( dt, *this );
	}

	stageSurf.SetTransformation( cam.GetTransform() );

	CollectingObjects();
}

void ActorTestScene::Draw( HDC hdc )
{
	Gdiplus::Graphics gfx( hdc );
	auto drawFuncs = [&]( HDC hdc, const Mat3<float>& camTransform )
	{
		stageSurf.DrawImageNonChromaGDI( hdc, stageImage, { 0,0 }, stageSize, { 0,0 }, stageImage.GetImageSize() );

		for ( const auto& pActor : actorPtrs )
		{
			pActor->SetTransform( camTransform );
			pActor->Draw( hdc );

#ifndef NDBUG
			const auto& pCollider = pActor->GetColliderPtr();
			pCollider->UpdateMatrix( camTransform );
			pCollider->Draw( gfx, { 144,255,255,255 } );
#endif // !NDBUG
		}
		for ( const auto& pBullet : bulletPtrs )
		{
			pBullet->SetTransform( camTransform );
			pBullet->Draw( hdc );

#ifndef NDBUG
			const auto& pCollider = pBullet->GetColliderPtr();
			pCollider->UpdateMatrix( camTransform );
			pCollider->Draw( gfx, { 144,255,255,255 } );
#endif // !NDBUG
		}


#ifndef NDBUG
		for ( const auto& pWall : wallPtrs )
		{
			const auto& pCollider = pWall->GetColliderPtr();
			pCollider->UpdateMatrix( camTransform );
			pCollider->Draw( gfx, { 144,255,0,255 } );
		}
#endif // !NDBUG

#ifndef NDBUG
		for ( const auto& pGround : groundPtrs )
		{
			const auto& pCollider = pGround->GetColliderPtr();
			pCollider->UpdateMatrix( camTransform );
			pCollider->Draw( gfx, { 255,0,0,255 } );
		}
#endif // !NDBUG

	};

	const float screenX = (sceneBottomRight.x - sceneTopLeft.x) / 2.0f;
	const float screenY = (sceneBottomRight.y - sceneTopLeft.y) / 2.0f;

	cam.Draw( hdc, { screenX, screenY }, drawFuncs );
}


