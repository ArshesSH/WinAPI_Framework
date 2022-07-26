#include "Game.h"

Game::Game()
{
	//pScenes.push_back( std::make_unique<TestScene>( screenWidth, screenHeight, ct ) );
	pScenes.push_back( std::make_unique<ActorTestScene>( screenWidth, screenHeight, ct ) );
	curScene = pScenes.begin();
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
	drawManager.DrawMain( hdc, screenRect, isScreenChanged,
		[this]( HDC hdc )
		{
			(*curScene)->Draw( hdc );
		}
	);
	if ( isScreenChanged )
	{
		isScreenChanged = false;
	}
}

void Game::UpdateModel()
{
	float dt = ft.Mark();
	RefreshScreen();
	(*curScene)->Update( dt, *this );
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

void Game::CycleScenes()
{
	if ( ++curScene == pScenes.end() )
	{
		curScene = pScenes.begin();
	}
}

void Game::ReverseCycleScenes()
{
	if ( curScene == pScenes.begin() )
	{
		curScene = pScenes.end() - 1;
	}
	else
	{
		--curScene;
	}
}
