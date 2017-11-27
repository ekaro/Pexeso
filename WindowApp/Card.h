#pragma once
#include <windows.h>

class Card
{
public:
	Card(HDC hdc, int x, int y, COLORREF CardColor, int RestartButtonOffset);
	void DrawCard();
private:
	int left;
	int top;
	int CardWidth = 100;
	int CardHeight = 150;
	HDC handle;
	COLORREF color;
	int offset;
};