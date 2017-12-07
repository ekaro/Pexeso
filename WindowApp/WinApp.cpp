#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <Windowsx.h>
#include <algorithm>
#include <random>
#include "Deck.h"

// Global variables

// The main window class name
static TCHAR szWindowClass[] = _T("WinApp");

// The string in the title bar
static TCHAR szTitle[] = _T("Window Application");

// Forward declaration of functions included in this module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Colors
HBRUSH hBrushBlack = CreateSolidBrush(RGB(0, 0, 0));

// Window properties
int offset = 100;
int WindowWidth = 500;
int WindowHeight = 600 + offset;
RECT CardRect;

int xPos;
int yPos;
int Card;
int FirstCard;
int SecondCard;

int RestartButtonWidth = 200;
int RestartButtonHeight = offset;
HWND RestartButton;

Deck NewDeck(offset);

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
/*
HWND RestartButton = CreateWindow(
	L"BUTTON",  // Predefined class; Unicode assumed 
	L"Restart",      // Button text 
	WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
	0,         // x position 
	0,         // y position 
	RestartButtonWidth,        // Button width
	RestartButtonHeight,        // Button height
	hWnd,     // Parent window
	NULL,       // No menu.
	hInstance,
	NULL
);*/

if (IsWindow(hWnd))
{
	DWORD dwStyle = GetWindowLongPtr(hWnd, GWL_STYLE);
	DWORD dwExStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
	HMENU menu = GetMenu(hWnd);

	RECT rc = { 0, 0, WindowWidth, WindowHeight };

	AdjustWindowRectEx(&rc, dwStyle, menu ? TRUE : FALSE, dwExStyle);

	SetWindowPos(hWnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOMOVE);
}

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


#define ID_BUTTON 1

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR display_msg[] = _T("Message in window");

	switch (message)
	{
	case WM_CREATE:

		RestartButton = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"New Game",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			0,         // x position 
			0,         // y position 
			RestartButtonWidth,        // Button width
			RestartButtonHeight,        // Button height
			hWnd,     // Parent window
			(HMENU) ID_BUTTON,       
			NULL,
			NULL
		);

	case WM_COMMAND:

		if (LOWORD(wParam) == ID_BUTTON)
		{
			NewDeck.NewGame();
			NewDeck.RedrawClient(hWnd);				
		}
		break;

	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);

		NewDeck.DrawDeck(hdc, hWnd);

		EndPaint(hWnd, &ps);
		break;

	case WM_LBUTTONDOWN:

		xPos = GET_X_LPARAM(lParam);
		yPos = GET_Y_LPARAM(lParam);

		Card = NewDeck.GetCardIndex(hWnd, xPos, yPos);

		NewDeck.CompareCards(hWnd, Card);

		NewDeck.Cards[Card].Clicked(hWnd);
	
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