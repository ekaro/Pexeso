#include "Deck.h"
#include <algorithm>

Deck::Deck(HWND hWnd, int ButtonOffset)
	:
	handle(hWnd),
	offset(ButtonOffset)
{
	Cards.resize(20);
	NewGame();
}

bool Deck::Card::Exposed = false;

void Deck::ResizeDeck()
{
	RECT ClientRect;
	int CurrentCardWidth;
	int CurrentCardHeight;

	::GetClientRect(handle, &ClientRect);
	CurrentCardWidth = (ClientRect.right - ClientRect.left) / 5;
	CurrentCardHeight = (ClientRect.bottom - ClientRect.top - offset) / 4;

	int index = 0;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			Cards[index].left = CurrentCardWidth * j;
			Cards[index].top = offset + CurrentCardHeight * i;
			Cards[index].CardWidth = CurrentCardWidth;
			Cards[index].CardHeight = CurrentCardHeight;

			if (Cards[index].Exposed == true)
			{
				Cards[index].Color = Cards[index].Blue;
			}
			else if (Cards[index].Exposed == false)
			{
				Cards[index].Color = Cards[index].Green;
			}

			index++;
		}
	}
}

void Deck::NewGame()
{
	for (int i = 0; i < Cards.size(); i++)
	{
		CardNums[i] = i % 10;
	}

	std::shuffle(std::begin(CardNums), std::end(CardNums), rng);

	for (int i = 0; i < Cards.size(); i++)
	{
		Cards[i].Number = CardNums[i];
	}
}

void Deck::DrawDeck(HDC hdc)
{
	ResizeDeck();

	for (Card c : Cards)
	{
		if (c.Exposed)
		{
			SelectObject(hdc, GetStockObject(DC_BRUSH));
			SetDCBrushColor(hdc, c.Blue);
		}
		else
		{
			SelectObject(hdc, GetStockObject(DC_BRUSH));
			SetDCBrushColor(hdc, c.Green);
		}

		c.DrawCard(hdc, c.CardWidth, c.CardHeight, c.Color);
	}
}


/*
Deck::Card::Card()
{
	//Color = Green;
	bool Deck::Card::Exposed = false;
}*/

void Deck::Card::DrawCard(HDC hdc, int CardWidth, int CardHeight, COLORREF CardColor)
{
	//SelectObject(hdc, GetStockObject(DC_BRUSH));
	//SetDCBrushColor(hdc, CardColor);
	Rectangle(hdc, left, top, left + CardWidth, top + CardHeight);
}

/*
void Deck::Card::Clicked()
{
	Exposed = true;
	Color = Blue;
}*/