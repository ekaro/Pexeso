#include "Deck.h"

Deck::Deck(int ButtonOffset)
	:
	offset(ButtonOffset)
{
	Cards.resize(20);
	NewGame();
	CardFont = CreateFont(70, 0, 0, 0, 300, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("Arial"));	
}

void Deck::ResizeDeck(HWND hWnd)
{
	RECT ClientRect;
	int CurrentCardWidth;
	int CurrentCardHeight;

	::GetClientRect(hWnd, &ClientRect);
	CurrentCardWidth = (ClientRect.right - ClientRect.left) / 5;
	CurrentCardHeight = (ClientRect.bottom - ClientRect.top - offset) / 4;

	int i = 0;

	for (int m = 0; m < rows; m++)
	{
		for (int n = 0; n < columns; n++)
		{
			Cards[i].left = CurrentCardWidth * n;
			Cards[i].top = offset + CurrentCardHeight * m;
			Cards[i].CardWidth = CurrentCardWidth;
			Cards[i].CardHeight = CurrentCardHeight;
			
			i++;
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
		if (Cards[i].Exposed == true)
		{
			Cards[i].Color = Cards[i].Blue;
			
		}
		else
		{
			Cards[i].Color = Cards[i].Green;
		}

		Cards[i].DrawCard(hdc, Cards[i].CardWidth, Cards[i].CardHeight, Cards[i].Color);

		if (Cards[i].Exposed == true) 
		{
			DrawNum(hdc, Cards[i]);
		}			
	}
}

void Deck::DrawNum(HDC hdc, Card card)
{
	std::wstring CardNumber = std::to_wstring(card.Number);

	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkColor(hdc, card.Color);
	SelectObject(hdc, CardFont);

	RECT CardRect = card.GetRect();
	DrawText(hdc, CardNumber.c_str(), _tcslen(CardNumber.c_str()), &CardRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void Deck::Card::DrawCard(HDC hdc, int CardWidth, int CardHeight, COLORREF CardColor)
{
	SelectObject(hdc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(hdc, CardColor);
	Rectangle(hdc, left, top, left + CardWidth, top + CardHeight);			
}

RECT Deck::Card::GetRect()
{
	return { left, top, left + CardWidth, top + CardHeight };
}

int Deck::GetCardIndex(HWND hWnd, int x, int y)
{
	RECT ClientRect;
	::GetClientRect(hWnd, &ClientRect);
	int CurrentCardWidth = (ClientRect.right - ClientRect.left) / 5;
	int CurrentCardHeight = (ClientRect.bottom - ClientRect.top - offset) / 4;
	int Card = (x / CurrentCardWidth) + ((y - offset) / CurrentCardHeight) * 5;

	return Card;   
}
