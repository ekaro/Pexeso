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
		Cards[i].Exposed = false;
	}
	
	Turns = 0; 
	State = 0;
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

	DrawTurns(hdc, hWnd);
}

void Deck::DrawTurns(HDC hdc, HWND hWnd)
{
	std::wstring TurnsString = std::to_wstring(Turns);

	SetTextColor(hdc, RGB(0, 255, 0));
	SetBkColor(hdc, RGB(0, 0, 0));
	SelectObject(hdc, CardFont);

	TextOut(hdc, 220, 5, TurnsMsg, _tcslen(TurnsMsg));
	TextOut(hdc, 400, 5, TurnsString.c_str(), _tcslen(TurnsString.c_str()));

	if (Turns < 10)
	{
		SelectObject(hdc, GetStockObject(DC_BRUSH));
		SetDCBrushColor(hdc, RGB(0, 0, 0));
		Rectangle(hdc, 435, 5, 480, 75);
	}
	
	InvalidateRect(hWnd, &TurnsRect, false);
}

void Deck::DrawNum(HDC hdc, Card card)
{
	std::wstring CardNumber = std::to_wstring(card.Number);

	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkColor(hdc, card.Color);
	SelectObject(hdc, CardFont);

	RECT CardRect = card.GetRect();
	DrawText(hdc, CardNumber.c_str(), 1, &CardRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void Deck::CompareCards(HWND hWnd, int Card) 
{
	if (Cards[Card].Exposed == false)
	{
		if (State == 0)
		{
			FirstCard = Card;
			Cards[FirstCard].Exposed = true;
			State = 1;
		}
		else if (State == 1)
		{
			SecondCard = Card;
			Cards[SecondCard].Exposed = true;
			State = 2;
			
		}
		else
		{
			Turns++;
			if (CardNums[FirstCard] != CardNums[SecondCard])
			{
				Cards[FirstCard].Exposed = false;
				Cards[SecondCard].Exposed = false;
				CardRect = Cards[FirstCard].GetRect();
				InvalidateRect(hWnd, &CardRect, false);
				CardRect = Cards[SecondCard].GetRect();
				InvalidateRect(hWnd, &CardRect, false);
			}
			FirstCard = Card;
			Cards[FirstCard].Exposed = true;
			State = 1;
		}
	}
}

void Deck::Card::DrawCard(HDC hdc, int CardWidth, int CardHeight, COLORREF CardColor)
{
	SelectObject(hdc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(hdc, CardColor);
	Rectangle(hdc, left, top, left + CardWidth, top + CardHeight);			
}

void Deck::Card::Clicked(HWND hWnd)
{
	RECT CardRect = GetRect();

	InvalidateRect(hWnd, &CardRect, false);
}

RECT Deck::Card::GetRect() const
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
