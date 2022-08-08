#pragma once

#include "framework.h"
#include <commdlg.h>
#include <string>

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

        hBottomWnd = CreateWindow( L"BottomWnd", L"View2", WS_POPUP | WS_CAPTION | WS_THICKFRAME | WS_VISIBLE,
            0, mainWndSize.cy, bottomWndSize.cx, bottomWndSize.cy, hWnd, NULL, hInst, NULL );

        clientRect = { 0,0,mainWndSize.cx, mainWndSize.cy };
        AdjustWindowRect( &clientRect, WS_OVERLAPPEDWINDOW, TRUE );
        GetWindowRect( hMenuWnd, &clientRect );

        hMenuWnd = CreateDialog( hInst, MAKEINTRESOURCE( dialogNum ), hWnd, dialogFunc );
        MoveWindow( hMenuWnd, mainWndSize.cx, 0, menuDlgWidth, wndSize.cy, TRUE );
    }

    ~EditorFrame()
    {
        DestroyWindow( hMenuWnd );
        DestroyWindow( hBottomWnd );
    }

    void FileOpenFrame( HWND& hWnd )
    {
        ZeroMemory( &ofn, sizeof( ofn ) );
        ofn.lStructSize = sizeof( ofn );
        ofn.hwndOwner = hWnd;
        ofn.lpstrTitle = L"파일을 선택하세요";
        ofn.lpstrFile = &fileName[0];
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
        ofn.lpstrFile = &fileName[0];
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
        MessageBox( 0, &fileName[0], L"모든 파일", 0 );
    }
    virtual void FileSave()
    {
        MessageBox( 0, &fileName[0], L"모든 파일", 0 );
    }

private:
    OPENFILENAME ofn;
    std::wstring fileName;

    HWND hMenuWnd;
    HWND hBottomWnd;

    SIZE wndSize;
    SIZE mainWndSize;
    SIZE bottomWndSize;
    static constexpr int menuDlgWidth = 300;
    RECT clientRect;
};