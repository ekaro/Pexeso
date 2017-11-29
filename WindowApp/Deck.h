#pragma once
#include <windows.h>
#include <vector>

class Deck
{
private:
	class Card
	{
	public:
		Card() = default;
		void DrawCard(HDC hdc);
		int offset = 100;  // offset is specified for every card because it is problematic to pass parameters from parent deck to card
		int left = 0;
		int top = offset;
		int CardWidth = 100;
		int CardHeight = 150;
		COLORREF Color = (RGB(0, 255, 0));
	private:
		HDC handle;
	};
public:
	Deck(HWND hWnd);
	void DrawDeck(HDC hdc);
	std::vector<Card> Cards;
private:
	HWND handle;
	int rows = 4;
	int columns = 5;
};