#pragma once
#include <windows.h>
#include <vector>

class Deck
{
private:
	class Card
	{
	public:
		Card(int x, int y, COLORREF CardColor);
		void DrawCard(HDC hdc);
		int CardWidth = 100;
		int CardHeight = 150;
		COLORREF color;
	private:
		HDC handle;
		int left;
		int top;		
	};
public:
	Deck(HDC hdc, int ButtonOffset);
	void GenerateDeck();
	void DrawDeck(HDC hdc);
private:
	HDC handle;
	int offset;
	int x;
	int y;
	int rows = 4;
	int columns = 5;
	std::vector<Card> Cards;
};