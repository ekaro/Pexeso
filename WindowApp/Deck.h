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
		void DrawCard();
		int CardWidth = 100;
		int CardHeight = 150;
		COLORREF color;
	private:
		int left;
		int top;		
	};
public:
	Deck(HDC hdc, int ButtonOffset);
	void GenerateDeck();
private:
	HDC handle;
	int offset;
	int rows = 4;
	int columns = 5;
	std::vector<Card> Cards;
};