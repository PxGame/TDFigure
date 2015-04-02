# include "main.h"

void MsgBoxEx(LPTSTR msg, ...)
{
		TCHAR szBuffer[1024];
		ZeroMemory(szBuffer,1024);
		int nWritten = 0;
		va_list args;
		va_start(args, msg);//获取msg后面的参数列表
		nWritten = _vsntprintf_s(szBuffer, 1024, msg, args);
		va_end(args);//释放

		//_tcscat_s(szBuffer, TEXT("[DbgMsg]"));
		MessageBox(NULL, msg, TEXT("OutMsg"), MB_OK);
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	if (! InitWndClass(hInstance))
	{
		int err = GetLastError();
		MsgBoxEx(TEXT("InitWndClass error code:%d"), err);
		return 0;
	}

	if (! InitMainWnd(hInstance))
	{
		int err = GetLastError();
		MsgBoxEx(TEXT("InitMainWnd error code:%d"), err);
		return 0;
	}

	ShowMainWnd(nShowCmd);

	return WinMsgLoop();
}

BOOL InitWndClass(HINSTANCE hInstance)
{
	WNDCLASS wnd;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wnd.hInstance = hInstance;
	wnd.lpfnWndProc = WindowProc;
	wnd.lpszClassName = g_lpClassName;
	wnd.lpszMenuName = NULL;
	wnd.style = CS_HREDRAW | CS_VREDRAW | CS_DROPSHADOW;

	if (!RegisterClass(&wnd))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL InitMainWnd(HINSTANCE hInstance)
{
	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
		//WS_CAPTION | WS_SYSMENU	| WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	RECT rect = { 0, 0, 600, 600 };
	AdjustWindowRect(&rect, dwStyle, NULL);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	POINT pt;
	pt.x = (g_ScrWidth - width) / 2;
	pt.y = (g_ScrHeight - height) / 2;
	
	g_hMainWnd = CreateWindow(
		g_lpClassName,
		TEXT("TestWindow"),
		dwStyle,
		pt.x, pt.y,
		width, height,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (g_hMainWnd == NULL)
	{
		return FALSE;
	}

	return TRUE;
}

void ShowMainWnd(int nCmdShow)
{
	ShowWindow(g_hMainWnd, nCmdShow);
	UpdateWindow(g_hMainWnd);
}

int WinMsgLoop()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		return OnCommand(hwnd, wParam, lParam);
	case WM_PAINT:
		return OnPaint(hwnd, wParam, lParam);
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT OnCommand(_In_ HWND hwnd, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	return DefWindowProc(hwnd, WM_COMMAND, wParam, lParam);
}

# include "BarChart.h"


LRESULT OnPaint(_In_ HWND hwnd, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	HDC hdc = NULL;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hwnd, &ps);

	//POINT pt = { 100, 300 };
	//DrawCube(hdc, pt, 200, 50, 100, RGB(100, 0, 0));
	RECT rt;
	GetClientRect(hwnd, &rt);
	//CTDFigure fg;
	//fg.Init(hdc, rt, 100);
	//fg.DrawDc();

	CBarChart bc;
	vector<BARCHARTITEM> item;
	BARCHARTITEM hh = { 20, TEXT("向牧") , RGB(0,255,0)};
	item.push_back(hh);
	hh.item.value = 30;
	hh.rgbColor = RGB(255,0,0);
	item.push_back(hh);
	hh.item.value = 40;
	hh.rgbColor = RGB(0, 0, 255);
	item.push_back(hh);
	//hh.item.value = 50;
	//hh.rgbColor = RGB(100, 100, 100);
	//item.push_back(hh);
	//hh.item.value = 1000;
	//hh.rgbColor = RGB(200, 200, 200);
	//item.push_back(hh);
	//hh.item.value = 200;
	//hh.rgbColor = RGB(255, 255, 255);
	//item.push_back(hh);

	BOOL bRet = FALSE;
	bRet = bc.Init(hdc, rt, 50, 10, item, 10);
	if (bRet == TRUE)
	{
		bc.DrawDc();
	}

	EndPaint(hwnd, &ps);

	return 0;
}

