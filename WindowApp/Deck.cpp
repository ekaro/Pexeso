#include "Deck.h"

Deck::Deck(HDC hdc, int ButtonOffset)
	:
	handle(hdc),
	offset(ButtonOffset)
{
}

void Deck::GenerateDeck()
{
	int index = 0;
	int x = 0;
	int y = offset;
	COLORREF Green = (RGB(0, 255, 0));

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			Cards[index] = Deck::Card(x, y, Green);
			x += Cards[index].CardWidth;
			index++;
		}
		x = 0;
		y += Cards[index].CardWidth;
	}
}

void Deck::DrawDeck(HDC hdc)
{
	for (int i = 0; i < rows; i++)
	{
		Cards[i].DrawCard(hdc);
	}
}

Deck::Card::Card(int x, int y, COLORREF CardColor)
	:
	left(x),
	top(y),
	color(CardColor)
{
}

void Deck::Card::DrawCard(HDC hdc)
{
	SelectObject(hdc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(hdc, color);
	Rectangle(hdc, left, top, left + CardWidth, top + CardHeight);
}