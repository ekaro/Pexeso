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
COLORREF Black = (RGB(0, 0, 0));
COLORREF White = (RGB(255, 255, 255));
COLORREF Red = (RGB(255, 0, 0));
COLORREF Green = (RGB(0, 255, 0));

// Window properties
int offset = 100;
int WindowWidth = 500;
int WindowHeight = 600 + offset;
RECT ClientRect;

int CardAreaWidth = WindowWidth;
int CardAreaHeight = WindowHeight - offset;

int xPos;
int yPos;

int CurrentCardAreaWidth;
int CurrentCardAreaHeight;
int CardW;
int CardH;
int Card;

int RestartButtonWidth = 200;
int RestartButtonHeight = offset;


void DrawCards(HDC handle, int CardWidth, int CardHeight, COLORREF Color)
{
	
	int left = 0;
	int top = 0;
	int right = CardWidth;
	int bottom = CardHeight;
	
	int rows = 4;
	int columns = 5;
	
	//    Select DC_BRUSH so you can change the brush color from the 
	//    default WHITE_BRUSH to any other color
	//SelectObject(handle, GetStockObject(DC_BRUSH));
	//SetDCBrushColor(handle, Color);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			Rectangle(handle, left, top + offset, right, bottom + offset);
			left += CardWidth;
			right += CardWidth;
		}
		left = 0;
		top += CardHeight;
		right = CardWidth;
		bottom += CardHeight;	
	}
}

void NewGame(HDC handle, HWND hWnd)
{
	int Deck[20];
	// fill up array as 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2.. (will be 10 groups)
	for (int i = 0; i < 20; i++)
	{
		Deck[i] = i % 10;
	}

	// and randomly rearrange order
	// shuffle array;
	auto rng = std::default_random_engine{};
	std::shuffle(std::begin(Deck), std::end(Deck), rng);

	bool Exposed[20] = {false};
	

	// Resing window
	::GetClientRect(hWnd, &ClientRect);
	CurrentCardAreaWidth = ClientRect.right - ClientRect.left;
	CurrentCardAreaHeight = ClientRect.bottom - ClientRect.top - offset;
	CardW = CurrentCardAreaWidth / 5;
	CardH = CurrentCardAreaHeight / 4;

	// Rectangle(hdc, 0, 0, Width/5, Height/5);
	DrawCards(handle, CardW, CardH, Green);
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
	int State = 0;
	Deck Deck(hWnd);

	switch (message)
	{
	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);

		// Here your application is laid out.
		// For this introduction, we just print out display message
		// in the top left corner.
		SetTextColor(hdc, RGB(255, 0, 0)); 
		SetBkColor(hdc, RGB(0, 0, 0));
		TextOut(hdc, RestartButtonWidth, 10, display_msg, _tcslen(display_msg));
		// End application-specific layout section.
		
		// Resing window
		//::GetClientRect(hWnd, &ClientRect);
		//CardAreaWidth = ClientRect.right - ClientRect.left;
		//CardAreaHeight = ClientRect.bottom - ClientRect.top - offset;
		
		// Rectangle(hdc, 0, 0, Width/5, Height/5);
		//NewGame(hdc, hWnd);
		// DrawCards(hdc, CardAreaWidth/5, CardAreaHeight/4, Green);
	
		Deck.DrawDeck(hdc);
		
		EndPaint(hWnd, &ps);
		break;

	case WM_CREATE:
		
		break;
	
	case WM_LBUTTONDOWN:

		xPos = GET_X_LPARAM(lParam);
		yPos = GET_Y_LPARAM(lParam);

		::GetClientRect(hWnd, &ClientRect);
		CurrentCardAreaWidth = ClientRect.right - ClientRect.left;
		CurrentCardAreaHeight = ClientRect.bottom - ClientRect.top - offset;
		CardW = CurrentCardAreaWidth / 5;
		CardH = CurrentCardAreaHeight / 4;

		Card = (xPos / CardW) + ((yPos - offset) / CardH) * 5;
		
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