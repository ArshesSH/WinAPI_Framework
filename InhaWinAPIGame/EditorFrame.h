#pragma once

#include "framework.h"
#include <commdlg.h>
#include <string>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

class EditorFrame
{
public:
    EditorFrame( HWND& hWnd, HINSTANCE hInst, UINT dialogNum, DLGPROC dialogFunc )
    {
        wndSize.cx = GetSystemMetrics( SM_CXSCREEN );
        wndSize.cy = GetSystemMetrics( SM_CYSCREEN );
        mainWndSize.cx = LONG(wndSize.cx * 0.75f);
        mainWndSize.cy = LONG((wndSize.cy - 30) * 0.75f);
        bottomWndSize.cx = mainWndSize.cx;
        bottomWndSize.cy = LONG( (wndSize.cy - 30) * 0.25f );

        MoveWindow( hWnd, 0, 0, mainWndSize.cx, mainWndSize.cy, TRUE );

        hBottomWnd = CreateWindowEx( WS_EX_CLIENTEDGE, _T("BottomWndClass"), L"View2", WS_POPUP | WS_CAPTION | WS_THICKFRAME | WS_VISIBLE,
            0, mainWndSize.cy, bottomWndSize.cx, bottomWndSize.cy, hWnd, NULL, hInst, NULL );

        clientRect = { 0,0,mainWndSize.cx, mainWndSize.cy };
        AdjustWindowRect( &clientRect, WS_OVERLAPPEDWINDOW, TRUE );
        GetWindowRect( hMenuWnd, &clientRect );

        hMenuWnd = CreateDialog( hInst, MAKEINTRESOURCE( dialogNum ), hWnd, dialogFunc );
        MoveWindow( hMenuWnd, mainWndSize.cx, 0, menuDlgWidth, wndSize.cy, TRUE );
    }

    virtual ~EditorFrame()
    {
        DestroyWindow( hMenuWnd );
        DestroyWindow( hBottomWnd );
    }

    virtual void InitWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) = 0;
    virtual void InitBottomProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) = 0;
    virtual void InitMenuProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) = 0;

    virtual void CaptureWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) = 0;
    virtual void CaptureBottomWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) = 0;
    virtual void CaptureMenuProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) = 0;

    virtual void Update() = 0;
    virtual void Draw( HDC hdc ) = 0;
    virtual void DrawBottomWnd( HDC hdc ) = 0;

    void FileOpenFrame( HWND& hWnd )
    {
        ZeroMemory( &ofn, sizeof( ofn ) );
        ofn.lStructSize = sizeof( ofn );
        ofn.hwndOwner = hWnd;
        ofn.lpstrTitle = L"파일을 선택하세요";
        ofn.lpstrFile = fileName;
        ofn.lpstrFilter = L"모든 파일(*.*)\0*.*\0";
        ofn.nMaxFile = MAX_PATH;
        ofn.nMaxFileTitle = MAX_PATH;

        if ( GetOpenFileName( &ofn ) != 0 )
        {
            switch ( ofn.nFilterIndex )
            {
            case 1:
                {
                    FileOpen();
                }
                break;
            default:
                break;
            }
        }
    }
    void FileSaveFrame( HWND& hWnd )
    {
        ZeroMemory( &ofn, sizeof( ofn ) );
        ofn.lStructSize = sizeof( ofn );
        ofn.hwndOwner = hWnd;
        ofn.lpstrTitle = L"저장할 위치를 선택하세요";
        ofn.lpstrFile = fileName;
        ofn.lpstrFilter = L"모든 파일(*.*)\0*.*\0";
        ofn.nMaxFile = MAX_PATH;
        ofn.nMaxFileTitle = MAX_PATH;
        if ( GetSaveFileName( &ofn ) != 0 )
        {
            switch ( ofn.nFilterIndex )
            {
            case 1:
                {
                    FileSave();
                }
                break;
            }
        }
    }

    void MoveEditor(HWND& hWnd)
    {
        GetWindowRect( hWnd, &clientRect );
        MoveWindow( hWnd, clientRect.left, clientRect.top, clientRect.right, clientRect.bottom, TRUE );
        MoveWindow( hMenuWnd, clientRect.right, clientRect.top, menuDlgWidth, wndSize.cy, TRUE );
        MoveWindow( hBottomWnd, clientRect.left, clientRect.bottom, bottomWndSize.cx, bottomWndSize.cy, TRUE );
    }

protected:
    virtual void FileOpen()
    {
        MessageBox( 0, fileName, L"모든 파일", 0 );
    }
    virtual void FileSave()
    {
        MessageBox( 0, fileName, L"모든 파일", 0 );
    }

protected:
    OPENFILENAME ofn;
    //std::wstring fileName;
    TCHAR fileName[MAX_PATH];

    HWND hMenuWnd;
    HWND hBottomWnd;

    SIZE wndSize;
    SIZE mainWndSize;
    SIZE bottomWndSize;
    static constexpr int menuDlgWidth = 300;
    RECT clientRect;

    bool isMainWndInit = false;
    bool isSubWndInit = false;
    bool isMenuInit = false;
};