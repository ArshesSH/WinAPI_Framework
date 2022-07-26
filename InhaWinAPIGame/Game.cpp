#include "Game.h"

#include "Vec2.h"
#include "Surface.h"

Game::Game()
{
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