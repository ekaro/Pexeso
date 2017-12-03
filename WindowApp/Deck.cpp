#include "Deck.h"
#include <algorithm>

Deck::Deck(int ButtonOffset)
	:
	//handle(hWnd),
	offset(ButtonOffset)
{
	Cards.resize(20);
	NewGame();
}

bool Deck::Card::Exposed = false;

void Deck::ResizeDeck(HWND hWnd)
{
	RECT ClientRect;
	int CurrentCardWidth;
	int CurrentCardHeight;

	::GetClientRect(hWnd, &ClientRect);
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
			/*
			if (Cards[index].Exposed == true)
			{
				Cards[index].Color = Cards[index].Blue;
			}
			else if (Cards[index].Exposed == false)
			{
				Cards[index].Color = Cards[index].Green;
			}*/

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

void Deck::DrawDeck(HDC hdc, HWND hWnd)
{
	ResizeDeck(hWnd);

	for (int i = 0; i < 20; i++)
	{
		/*
		if (c.Exposed)
		{
			SelectObject(hdc, GetStockObject(DC_BRUSH));
			SetDCBrushColor(hdc, c.Blue);
		}
		else
		{
			SelectObject(hdc, GetStockObject(DC_BRUSH));
			SetDCBrushColor(hdc, c.Green);
		}*/

		Cards[i].DrawCard(hdc, Cards[i].CardWidth, Cards[i].CardHeight, Cards[i].Color);
		
		std::wstring CardNumber = std::to_wstring(Cards[i].Number);
		SetTextColor(hdc, RGB(255, 0, 0));
		SetBkColor(hdc, RGB(0, 0, 0));
		TextOut(hdc,
			Cards[i].left + Cards[i].CardWidth / 2, Cards[i].top + Cards[i].CardHeight / 2,
			CardNumber.c_str(), _tcslen(CardNumber.c_str()));
	}
}

Deck::Card::Card()
{
	//Color = Green;
	//Exposed = false;
}

void Deck::Card::DrawCard(HDC hdc, int CardWidth, int CardHeight, COLORREF CardColor)
{
	//SelectObject(hdc, GetStockObject(DC_BRUSH));
	//SetDCBrushColor(hdc, CardColor);
	//Rectangle(hdc, left, top, left + CardWidth, top + CardHeight);

	for (int i = 0; i < 100; i++)
	{
		if (Exposed == true) 
		{
			SetPixel(hdc, left+i, top, Blue);
		}
		else
		{
			SetPixel(hdc, left + i, top, Green);
		}
	}
	/*
	TCHAR display_msg[] = _T("Card");
	SetTextColor(hdc, RGB(255, 0, 0));
	SetBkColor(hdc, RGB(0, 0, 0));
	TextOut(hdc,
		left+CardWidth/2, top+CardHeight/2,
		display_msg, _tcslen(display_msg));*/
}

/*
void Deck::Card::Clicked()
{
	Exposed = true;
	Color = Blue;
}*/