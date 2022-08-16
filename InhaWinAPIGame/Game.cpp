#include "Game.h"

Game::Game()
{
	pScenes.push_back( std::make_unique<ActorTestScene>( screenWidth, screenHeight, ct ) );
	curScene = pScenes.begin();
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
