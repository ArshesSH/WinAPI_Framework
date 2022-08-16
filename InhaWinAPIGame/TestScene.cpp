#include "TestScene.h"

#include "Game.h"
#include "Surface.h"

TestScene::TestScene( int sceneWidth, int sceneHeight, CoordinateTransformer& ct )
	:
	Scene( sceneWidth, sceneHeight ),
	imageTest( L"Images/awsom.bmp" ),
	imageTest2( L"Images/awsom.bmp" ),
	cam( ct ),
	dudeGravity( 9.8f ),
	testCollider( testRect )
{
	testPoly.emplace_back( 300, 0 );
	testPoly.emplace_back( 400, 0 );
	testPoly.emplace_back( 400, 100 );
	testPoly.emplace_back( 300, 100 );
	pColliders.push_back( &testCollider );
	pColliders.push_back( &testLineCollider );
	pColliders.push_back( &testCircleCollider );
}

void TestScene::Update( float dt, Game& game )
{

	/*dudeVel.y = dudeGravity.GetGravityVel( dudeVel, dt );
	dudePos += dudeVel * dt;*/
	delta = dt;
	if ( GetAsyncKeyState( 'A' ) & 0x8001 )
	{
		cam.MoveBy( dirLeft * dt * 200 );
	}
	else if ( GetAsyncKeyState( 'D' ) & 0x8001 )
	{
		cam.MoveBy( dirRight * dt * 200 );
	}
	if ( GetAsyncKeyState( 'W' ) & 0x8001 )
	{
		cam.MoveBy( dirUp * dt * 200 );
	}
	else if ( GetAsyncKeyState( 'S' ) & 0x8001 )
	{
		cam.MoveBy( dirDown * dt * 200 );
	}


	
	// Move
	{
		Vec2<float> nextPos = dudePos;
		if ( GetAsyncKeyState( VK_LEFT ) & 0x8001 )
		{
			nextPos= dudePos + dirLeft * dt * 200;
		}
		else if ( GetAsyncKeyState( VK_RIGHT ) & 0x8001 )
		{
			nextPos = dudePos + dirRight * dt * 200;
		}
		if ( GetAsyncKeyState( VK_UP ) & 0x8001 )
		{
			nextPos = dudePos + dirUp * dt * 200;
		}
		else if ( GetAsyncKeyState( VK_DOWN ) & 0x8001 )
		{
			nextPos = dudePos + dirDown * dt * 200;
		}

		ConvexCollider<float> nextCollider( nextPos, 100, 100 );
		isCollided = false;
		for ( auto c : pColliders )
		{
			if ( collisionManager.IsOverlapWithAABB( nextCollider, *c ) )
			{
				isCollided |= true;
			}
			else
			{
				isCollided |= false;
			}
		}

		if ( !isCollided )
		{
			dudeCollider.SetPos( dudePos );
			dudePos = nextPos;
		}
	}


	if ( GetAsyncKeyState( 'Q' ) & 0x8001 )
	{
		cam.SetScale( cam.GetScale() - (2.0f * dt) );
	}
	else if ( GetAsyncKeyState( 'E' ) & 0x8001 )
	{
		cam.SetScale( cam.GetScale() + (2.0f * dt) );
	}
}

void TestScene::Draw( HDC hdc )
{
	Gdiplus::Graphics gfx( hdc );
	//surf.DrawRectGDI( hdc, 30, 30, 100, 100, RGB( 255, 255, 255 ) );
	//surf.DrawImageNonChromaGDI( hdc, imageTest.GetHBitmap(), { 50,50 }, { 100,100 }, { 0,0 }, imageTest.GetImageSize() );
	//surf.DrawImageNonChromaPlus( gfx, imageTest2.GetImagePtr(), { 100,100 }, { 200,200 }, { 0,0 }, imageTest2.GetImageSize() );

	auto draws = [&]( HDC hdc, const Mat3<float>& camTransform )
	{
		auto transform1 = Mat3<float>::Translation( { 100, 100 } ) * Mat3<float>::Scale( 1 );
		//auto transform2 = Mat3<float>::Translation( { 200,200 } );
		surf.SetTransformation( camTransform );
		surf2.SetTransformation( camTransform );
		surf.DrawFillRectGDI( hdc, 0, 0, 100, 100, RGB( 255, 255, 255 ) );
		surf.DrawFillRectGDI( hdc, 100, 100, 300, 300, RGB( 255, 0, 0 ) );
		surf2.DrawImageNonChromaGDI( hdc, imageTest.GetHBitmap(), { 0,0 }, { 100,100 }, { 0,0 }, imageTest.GetImageSize() );
		surf.DrawFillRectPlus( gfx, { 300,0 }, { 100,100 }, Gdiplus::Color{ 255,255,255,0 } );
		surf2.DrawImageChromaPlus( gfx, imageTest2, dudePos, dudeSize, { 0,0 }, imageTest2.GetImageSize() );

		for ( auto& pCollider : pColliders )
		{
			pCollider->UpdateMatrix( camTransform );
			pCollider->Draw( gfx, { 144,255,255,255 } );
		}

		//dudeCollider.UpdateMatrix( camTransform );
		//dudeCollider.Draw( gfx, Gdiplus::Color{ 255,255,255,255 } );
	};

	const float screenX = ( sceneBottomRight.x - sceneTopLeft.x) / 2.0f;
	const float screenY = (sceneBottomRight.y - sceneTopLeft.y) / 2.0f;
	cam.Draw( hdc, { screenX, screenY }, draws );

	Surface<float> s;
	const auto camPos = cam.GetPos();
	const auto camScale = cam.GetScale();
	const auto dudeColliderPos = dudeCollider.GetPos();
	const std::wstring camPosStr = L"camPos: (" + std::to_wstring( camPos.x ) + L", " + std::to_wstring( camPos.y ) + L")";
	const std::wstring camScaleStr = L"camScale: " + std::to_wstring( cam.GetScale() );
	const std::wstring dudeVelStr = L"dudeVel: (" + std::to_wstring( dudeVel.x ) + L", " + std::to_wstring( dudeVel.y ) + L")";
	const std::wstring dudePosStr = L"dudePos: (" + std::to_wstring( dudePos.x ) + L", " + std::to_wstring( dudePos.y ) + L")";
	const std::wstring dudColStr = L"dudCollision: (" + std::to_wstring( dudeColliderPos.x ) + L", " + std::to_wstring( dudeColliderPos.y ) + L")";
	const std::wstring collideStr = (isCollided) ? L"true" : L"false";
	const std::wstring deltaStr = L"dt = " + std::to_wstring( delta );
	s.DrawStringPlus( gfx, camPosStr, { 0,0 }, { 255,255,255,255 } );
	s.DrawStringPlus( gfx, camScaleStr, { 0,20 }, { 255,255,255,255 } );
	s.DrawStringPlus( gfx, dudeVelStr, { 0,40 }, { 255,255,255,255 } );
	s.DrawStringPlus( gfx, dudePosStr, { 0,60 }, { 255,255,255,255 } );
	s.DrawStringPlus( gfx, dudColStr, { 0,80 }, { 255,255,255,255 } );
	s.DrawStringPlus( gfx, collideStr, { 0,100 }, { 255,255,255,255 } );
	s.DrawStringGDI( hdc, { 0,120 }, deltaStr );
}
