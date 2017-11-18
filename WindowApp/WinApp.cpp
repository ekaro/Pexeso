#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <Windowsx.h>
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

// Global variables

// The main window class name
static TCHAR szWindowClass[] = _T("WinApp");

// The string in the title bar
static TCHAR szTitle[] = _T("Window Application");

// Forward declaration of functions included in this module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Declaration of Button3 for WM_CREATE;
HWND Button3;

// Colors
HBRUSH hBrushBlack = CreateSolidBrush(RGB(0, 0, 0));
COLORREF Black = (RGB(0, 0, 0));
COLORREF White = (RGB(255, 255, 255));
COLORREF Red = (RGB(255, 0, 0));

// Window properties
int WindowWidth = 500;
int WindowHeight = 600;
RECT WindowRect;

int xPos;
int yPos;

void DrawCards(HDC handle, int CardWidth, int CardHeight)
{
	int left = 0;
	int top = 0;
	int right = CardWidth;
	int bottom = CardHeight;
	int rows = 4;
	int columns = 5;
	
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			Rectangle(handle, left, top, right, bottom);
			left += CardWidth;
			right += CardWidth;
		}
		left = 0;
		top += CardHeight;
		right = CardWidth;
		bottom += CardHeight;	
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	// wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hbrBackground = hBrushBlack;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}

	// The parameters to CreateWindow explained:
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_OVERLAPPEDWINDOW: the type of window to create
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// 500, 100: initial size (width, length)
	// NULL: the parent of this window
	// NULL: this application does not have a menu bar
	// hInstance: the first parameter from WinMain
	// NULL: not used in this application
	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		WindowWidth, WindowHeight,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (IsWindow(hWnd))
	{
		DWORD dwStyle = GetWindowLongPtr(hWnd, GWL_STYLE);
		DWORD dwExStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
		HMENU menu = GetMenu(hWnd);

		RECT rc = { 0, 0, WindowWidth, WindowHeight };

		AdjustWindowRectEx(&rc, dwStyle, menu ? TRUE : FALSE, dwExStyle);

		SetWindowPos(hWnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOMOVE);
	}

	HWND Button1 = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"Button1",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		100,         // x position 
		100,         // y position 
		100,        // Button width
		100,        // Button height
		hWnd,     // Parent window
		NULL,       // No menu.
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL
	);

	HWND Button2 = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"Button2",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		200,         // x position 
		100,         // y position 
		WindowWidth/5,        // Button width
		WindowHeight/5,        // Button height
		hWnd,     // Parent window
		NULL,       // No menu.
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL
	);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}

	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR display_msg[] = _T("Message in window");

	switch (message)
	{
	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);

		// Here your application is laid out.
		// For this introduction, we just print out display message
		// in the top left corner.
		SetTextColor(hdc, RGB(255, 0, 0)); 
		SetBkColor(hdc, RGB(0, 0, 0));
		TextOut(hdc, 5, 5, display_msg, _tcslen(display_msg));
		// End application-specific layout section.
		
		// Resing window
		::GetClientRect(hWnd, &WindowRect);
		WindowWidth = WindowRect.right - WindowRect.left;
		WindowHeight = WindowRect.bottom - WindowRect.top;

		//    Select DC_BRUSH so you can change the brush color from the 
		//    default WHITE_BRUSH to any other color
		SelectObject(hdc, GetStockObject(DC_BRUSH));
		SetDCBrushColor(hdc, Red);
		
		// Rectangle(hdc, 0, 0, Width/5, Height/5);
		DrawCards(hdc, WindowWidth/5, WindowHeight/4);
		
		EndPaint(hWnd, &ps);
		break;

	case WM_CREATE:

		Button3 = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"Button3",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			300,         // x position 
			100,         // y position 
			100,        // Button width
			100,        // Button height
			hWnd,     // Parent window
			NULL,       // No menu.
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL
		);
		break;
	
	case WM_LBUTTONDOWN:

		xPos = GET_X_LPARAM(lParam);
		yPos = GET_Y_LPARAM(lParam);

		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}