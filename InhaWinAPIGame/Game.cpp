#include "Game.h"

#include "Vec2.h"
#include "Surface.h"

Game::Game()
	:
	imageTest( L"Images/awsom.bmp" ),
	imageTest2( L"Images/awsom.bmp" )
{
}

// This function Call in Win32API's WM_PAINT, and draw everything about game
// Pass object's draw functions to drawmanger by lambda to draw on screen
// Also pass isScreenChanged, to change draw area when screen size changed
//
// �� �Լ��� window api�� WM_PAINT���� ȣ���մϴ�. �� �Լ��� ���Ӱ� ���õ� ��� �͵��� �׸��� �����Դϴ�.
// ��� ��ü���� draw�Լ����� drawmanger���� ���ٸ� ���� �����ϰ�, ��ũ���� ����ϵ��� �մϴ�.
// ���� isScreenChanged�� �����ؼ� â�� ũ�Ⱑ ����Ǿ��� ���, �̿� ���߾� ��µǵ��� �մϴ�.
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
					
					auto transform = Mat3<float>::Translation( { 100, 100 } ) * Mat3<float>::Scale( 2 );
					surf.ApplyTransformation( transform );
					surf.DrawRectGDI( hdc, 0, 0, 100, 100, RGB( 255, 255, 255 ) );
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