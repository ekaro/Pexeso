#include "Deck.h"

Deck::Deck(HWND hWnd)
	:
	handle(hWnd)
{
	Cards.resize(20);
	int num = Cards.size();

	int index = 0;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			Cards[index].left += Cards[index].CardWidth * j;
			Cards[index].top += Cards[index].CardHeight * i;
			index++;
		}		
	}
}

void Deck::DrawDeck(HDC hdc)
{
	for (Card c : Cards)
	{
		c.DrawCard(hdc);
	}
}


void Deck::Card::DrawCard(HDC hdc)
{
	SelectObject(hdc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(hdc, Color);
	Rectangle(hdc, left, top, left + CardWidth, top + CardHeight);
}