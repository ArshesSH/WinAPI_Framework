#include "ActorTestScene.h"

#include "Game.h"
#include "PlayerX.h"
#include "SigmaHead.h"

ActorTestScene::ActorTestScene( int sceneWidth, int sceneHeight, CoordinateTransformer& ct )
	:
	Scene( sceneWidth, sceneHeight, ct ),
	stageImage( L"Images/RockmanX5/OpeningStage.bmp" ),
	stageSize( stageImage.GetImageSize() * 2 )
{
	cam.SetPos( { 200.0f,240.0f } );
	cam.SetScale( 1.0f );
	actorPtrs.emplace_back( std::make_unique<PlayerX>( Vec2<float>{ 300.0f, 300.0f } ) );
	actorPtrs.emplace_back( std::make_unique<SigmaHead>( Vec2<float>{ 500.0f, 200.0f } ) );

	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>{500.0f, 30.0f}, 1000.0f, 50.0f ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>{500.0f, 500.0f}, 1000.0f, 50.0f ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>{-50.0f, 150.0f}, 50.0f, 300.0f ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>(1340,0), Vec2<float>(1650, 180)) );
	groundPtrs.emplace_back( std::make_unique<Ground>( Line<float>( { 1650, 181 }, { 2200, 245 } ) ) );
	groundPtrs.emplace_back( std::make_unique<Ground>( Line<float>( { 2200, 240 }, { 2700, 181 } ) ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>( 2700, 0 ), Vec2<float>( 2770, 180 ) ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>( 2770, 0 ), Vec2<float>( 2910, 60 ) ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>( 2910, 0 ), Vec2<float>( 3160, 180 ) ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>( 3160, 0 ), Vec2<float>( 3300, 100 ) ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>( 3300, 0 ), Vec2<float>( 3680, 180 ) ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>( 3680, 0 ), Vec2<float>( 3810, 100 ) ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>( 3810, 0 ), Vec2<float>( 4480, 180 ) ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>( 4480, 0 ), Vec2<float>( 4980, 355 ) ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>( 4980, 0 ), Vec2<float>( 5122, 244 ) ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>( 5122, 420 ), Vec2<float>( 7104, 500 ) ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>( 5122, 0 ), Vec2<float>( 5246, 220 ) ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>( 5246, 0 ), Vec2<float>( 5632, 120 ) ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>( 5632, 352 ), Vec2<float>( 5760, 500 ) ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>( 5632, 0 ), Vec2<float>( 5760, 226 ) ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>( 6046, 0 ), Vec2<float>( 6176, 226 ) ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>( 5760, 0 ), Vec2<float>( 6558, 140 ) ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>( 6558, 0 ), Vec2<float>( 6912, 74 ) ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>( 6688, 0 ), Vec2<float>( 6782, 140 ) ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>( 6912, 0 ), Vec2<float>( 7042, 140 ) ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>( 7042, 0 ), Vec2<float>( 7168, 252 ) ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>( 7552, 0 ), Vec2<float>( 7680, 252 ) ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>( 8128, 0 ), Vec2<float>( 8256, 252 ) ) );
	wallPtrs.emplace_back( std::make_unique<Wall>( Vec2<float>( 8256, 0 ), Vec2<float>( 8864, 100 ) ) );
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


		for ( auto i = actorPtrs.rbegin(); i != actorPtrs.rend(); ++i )
		{
			(*i)->SetTransform( camTransform );
			(*i)->Draw( hdc );

#ifndef NDBUG
			const auto& pCollider = (*i)->GetColliderPtr();
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


//		for ( const auto& pActor : actorPtrs )
//		{
//			pActor->SetTransform( camTransform );
//			pActor->Draw( hdc );
//
//#ifndef NDBUG
//			const auto& pCollider = pActor->GetColliderPtr();
//			pCollider->UpdateMatrix( camTransform );
//			pCollider->Draw( gfx, { 144,255,255,255 } );
//#endif // !NDBUG
//		}
	};

	const float screenX = (sceneBottomRight.x - sceneTopLeft.x) / 2.0f;
	const float screenY = (sceneBottomRight.y - sceneTopLeft.y) / 2.0f;

	cam.Draw( hdc, { screenX, screenY }, drawFuncs );
}


