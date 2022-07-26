#pragma once

#include "framework.h"
#include "Vec2.h"
#include <vector>
#include <ObjIdl.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")

class DrawManager
{
public:
	template<typename F>
	void DrawMain( HDC hdc, const RECT& clientRECT, bool isClientSizeChanged, F drawFunc )
	{
		HDC hMemDC;
		HBITMAP hOldBitmap;

		hMemDC = CreateCompatibleDC( hdc );
		if ( hDoubleBufferImage == nullptr || isClientSizeChanged )
		{
			// Create Bitmap Image for Double buffering
			hDoubleBufferImage = CreateCompatibleBitmap( hdc, clientRECT.right, clientRECT.bottom );
		}

		hOldBitmap = (HBITMAP)SelectObject( hMemDC, hDoubleBufferImage );

		FillRect( hMemDC, &clientRECT, (HBRUSH)GetStockObject( BLACK_BRUSH ) );

		/*Surface a;
		a.DrawString( hMemDC, L"Hello", pos, {255,255,0,0} );*/

		// Call Actual Draw functions
		drawFunc(hMemDC);

		BitBlt( hdc, 0, 0, clientRECT.right, clientRECT.bottom, hMemDC, 0, 0, SRCCOPY );
		SelectObject( hMemDC, hOldBitmap );
		DeleteObject( hMemDC );
	}

private:
	HBITMAP hDoubleBufferImage = nullptr;
};