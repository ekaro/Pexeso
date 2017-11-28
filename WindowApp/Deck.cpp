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
	int y = 0;
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

Deck::Card::Card(int x, int y, COLORREF CardColor)
	:
	left(x),
	top(y),
	color(CardColor)
{
}