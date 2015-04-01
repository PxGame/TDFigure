# pragma once
# include <windows.h>
# include <tchar.h>
# include <string>
using namespace std;

const int g_ScrWidth = GetSystemMetrics(SM_CXSCREEN);
const int g_ScrHeight = GetSystemMetrics(SM_CYSCREEN);

LPCTSTR g_lpClassName = TEXT("MainClass");
HWND g_hMainWnd = NULL;

void MsgBoxEx(LPTSTR msg, ...);

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd);

BOOL InitWndClass(HINSTANCE hInstance);

BOOL InitMainWnd(HINSTANCE hInstance);

void ShowMainWnd(int nCmdShow);

int WinMsgLoop();

LRESULT CALLBACK WindowProc(
	_In_  HWND hwnd,
	_In_  UINT uMsg,
	_In_  WPARAM wParam,
	_In_  LPARAM lParam
	);

LRESULT OnCommand(
	_In_  HWND hwnd,
	_In_  WPARAM wParam,
	_In_  LPARAM lParam
	);

LRESULT OnPaint(
	_In_  HWND hwnd,
	_In_  WPARAM wParam,
	_In_  LPARAM lParam
	);