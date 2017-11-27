#include "Card.h"

Card::Card(HDC hdc, int x, int y, COLORREF CardColor, int RestartButtonOffset)
	:
	left(x),
	top(y),
	handle(hdc),
	offset(RestartButtonOffset),
	color(CardColor)
{
}

void Card::DrawCard()
{
	SelectObject(handle, GetStockObject(DC_BRUSH));
	SetDCBrushColor(handle, color);
	Rectangle(handle, left, top + offset, left + CardWidth, top + CardHeight + offset);
}

