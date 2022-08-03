#include "Game.h"

#include "Vec2.h"
#include "Surface.h"

Game::Game()
	:
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
}

// This function Call in Win32API's WM_PAINT, and draw everything about game
// Pass object's draw functions to drawmanger by lambda to draw on screen
// Also pass isScreenChanged, to change draw area when screen size changed
//
// 이 함수는 window api의 WM_PAINT에서 호출합니다. 이 함수는 게임과 관련된 모든 것들을 그리는 역할입니다.
// 모든 객체들의 draw함수들은 drawmanger에게 람다를 통해 전달하고, 스크린에 출력하도록 합니다.
// 또한 isScreenChanged를 전달해서 창의 크기가 변경되었을 경우, 이에 맞추어 출력되도록 합니다.
void Game::ComposeFrame(HDC hdc)
{
	switch ( sceneType )
	{
	case Game::SceneType::SceneStart:
		{
		}
		break;
	case Game::SceneType::SceneStage:
		{
			drawManager.DrawMain( hdc, screenRect, isScreenChanged,
				[this]( HDC hdc )
				{
					Gdiplus::Graphics gfx( hdc );
					//surf.DrawRectGDI( hdc, 30, 30, 100, 100, RGB( 255, 255, 255 ) );
					//surf.DrawImageNonChromaGDI( hdc, imageTest.GetHBitmap(), { 50,50 }, { 100,100 }, { 0,0 }, imageTest.GetImageSize() );
					//surf.DrawImageNonChromaPlus( gfx, imageTest2.GetImagePtr(), { 100,100 }, { 200,200 }, { 0,0 }, imageTest2.GetImageSize() );
					
					auto draws = [&]( HDC hdc, const Mat3<float>& camTransform )
					{
						auto transform1 = Mat3<float>::Translation( { 100, 100 } ) * Mat3<float>::Scale( 1 );
						//auto transform2 = Mat3<float>::Translation( { 200,200 } );
						surf.SetTransformation( camTransform  );
						surf2.SetTransformation( camTransform  );
						surf.DrawRectGDI( hdc, 0, 0, 100, 100, RGB( 255, 255, 255 ) );
						surf.DrawRectGDI( hdc, 100, 100, 300, 300, RGB( 255, 0, 0 ) );
 						surf2.DrawImageNonChromaGDI( hdc, imageTest.GetHBitmap(), { 0,0 }, { 100,100 }, { 0,0 }, imageTest.GetImageSize() );
						surf.DrawFillRectPlus( gfx, { 300,0 }, { 100,100 }, Gdiplus::Color{ 255,255,255,0} );
						surf2.DrawImageChromaPlus( gfx, imageTest2, dudePos, dudeSize, { 0,0 }, imageTest2.GetImageSize() );
						dudeCollider.UpdateMatrix( camTransform );
						dudeCollider.Draw( gfx, Gdiplus::Color{ 144,255,0,255 } );
						testCollider.UpdateMatrix( camTransform );
						testCollider.Draw( gfx, Gdiplus::Color{255,255,255,255} );
					};

					const float screenX = (screenRect.right - screenRect.left) / 2.0f;
					const float screenY = (screenRect.bottom - screenRect.top) / 2.0f;
					cam.Draw( hdc, { screenX, screenY }, draws);
					
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
					s.DrawStringPlus( gfx, camPosStr, { 0,0 }, {255,255,255,255} );
					s.DrawStringPlus( gfx, camScaleStr, { 0,20 }, { 255,255,255,255 } );
					s.DrawStringPlus( gfx, dudeVelStr, { 0,40 }, { 255,255,255,255 } );
					s.DrawStringPlus( gfx, dudePosStr, { 0,60 }, { 255,255,255,255 } );
					s.DrawStringPlus( gfx, dudColStr, { 0,80 }, { 255,255,255,255 } );
					s.DrawStringPlus( gfx, collideStr, { 0,100 }, { 255,255,255,255 } );
				}
			);

			if ( isScreenChanged )
			{
				isScreenChanged = false;
			}
		}
		break;
	case Game::SceneType::SceneTest:
		{
			drawManager.DrawMain( hdc, screenRect, isScreenChanged,
				[this]( HDC hdc )
				{
					//testTriangulationScene.Draw( hdc );

				}
			);
		}
		break;
	case Game::SceneType::SceneResult:
		{
		}
		break;
	}
}

void Game::UpdateModel()
{
	switch ( sceneType )
	{
	case Game::SceneType::SceneStart:
		{
		}
		break;
	case Game::SceneType::SceneStage:
		{
			float dt = ft.Mark();
			RefreshScreen();

			/*dudeVel.y = dudeGravity.GetGravityVel( dudeVel, dt );
			dudePos += dudeVel * dt;*/

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

			if ( GetAsyncKeyState( VK_LEFT ) & 0x8001 )
			{
				dudePos += dirLeft * dt * 200;
			}
			else if ( GetAsyncKeyState( VK_RIGHT ) & 0x8001 )
			{
				dudePos += dirRight * dt * 200;
			}
			if ( GetAsyncKeyState( VK_UP ) & 0x8001 )
			{
				dudePos += dirUp * dt * 200;
			}
			else if ( GetAsyncKeyState( VK_DOWN ) & 0x8001 )
			{
				dudePos += dirDown * dt * 200;
			}
			dudeCollider.SetPos( dudePos );

			for ( auto c : pColliders )
			{

				//if ( dudeCollider.IsOverlapWithAABB( testCollider ) )
				if ( c->IsOverlapWithOBB( dudeCollider ) )
				{
					isCollided = true;
				}
				else
				{
					isCollided = false;
				}
			}




			if ( GetAsyncKeyState( 'Q' ) & 0x8001 )
			{
				cam.SetScale( cam.GetScale() - (2.0f  * dt) );
			}
			else if ( GetAsyncKeyState( 'E' ) & 0x8001 )
			{
				cam.SetScale( cam.GetScale() + (2.0f * dt) );
			}

		}
		break;
	case Game::SceneType::SceneTest:
		{
			float dt = ft.Mark();
			RefreshScreen();
			//testTriangulationScene.Update(dt, *this);
		}
		break;
	case Game::SceneType::SceneResult:
		{
		}
		break;
	}
}
void Game::RefreshScreen()
{
	if ( screenRect.right != oldScreenSize.right || screenRect.bottom != oldScreenSize.bottom )
	{
		isScreenChanged = true;

		oldScreenSize.left = screenRect.left;
		oldScreenSize.top = screenRect.top;
		oldScreenSize.right = screenRect.right;
		oldScreenSize.bottom = screenRect.bottom;
	}
}

void Game::SetClientSize( HWND hWnd, int width, int height )
{
	SetWindowPos( hWnd, nullptr, 0, 0, width, height, SWP_NOMOVE );

	RECT winRect;
	RECT screenRect;

	GetWindowRect( hWnd, &winRect );
	GetClientRect( hWnd, &screenRect );

	const int clientWidth = screenRect.right - screenRect.left;
	const int clientHeight = screenRect.bottom - screenRect.top;
	int winWidth = winRect.right - winRect.left;
	int winHeight = winRect.bottom - winRect.top;

	winWidth += winWidth - clientWidth;
	winHeight += winHeight - clientHeight;

	const int resolutionX = GetSystemMetrics( SM_CXSCREEN );
	const int resolutionY = GetSystemMetrics( SM_CYSCREEN );

	SetWindowPos( hWnd, nullptr, ((resolutionX / 2) - (winWidth / 2)),
		((resolutionY / 2) - (winHeight / 2)), winWidth, winHeight, NULL );
}
