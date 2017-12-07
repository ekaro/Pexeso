#pragma once
#include <windows.h>
#include <vector>
#include <random>
#include <algorithm>
#include <tchar.h>
#include <string> 

class Deck
{
private:
	class Card
	{
	public:
		Card() = default;
		void DrawCard(HDC hdc, int CardWidth, int CardHeight, COLORREF Color);
		int left = 0;
		int top;
		int CardWidth;
		int CardHeight;
		COLORREF Green = (RGB(0, 255, 0));
		COLORREF Blue = (RGB(0, 0, 255));
		COLORREF Color;
		bool Exposed = false;
		int Number;
		RECT GetRect() const;
		void Clicked(HWND hWnd);
	private:
		HDC handle;	
	};
public:
	Deck(int ButtonOffset);
	void DrawDeck(HDC hdc, HWND hWnd);    // drawing of every card in the deck
	void ResizeDeck(HWND hWnd);		   // resizing of deck according to current size of the window
	void NewGame();
	std::vector<Card> Cards;   // vector of all cards in the deck
	int CardNums[20];
	void DrawNum(HDC hdc, Card card);
	void DrawTurns(HDC hdc, HWND hWnd);
	void CompareCards(HWND hWnd, int Card);
	void RedrawClient(HWND hWnd);
	int GetCardIndex(HWND hWnd, int x, int y);   //returns index of card that has been clicked		
private:
	static constexpr RECT TurnsRect = { 400, 5, 500, 75};
	static constexpr TCHAR TurnsMsg[] = _T("Turns:");
	HWND handle;
	int columns = 5;
	int rows = 4;
	int offset;
	int FirstCard;
	int SecondCard;
	int State;
	int Turns;
	HFONT CardFont;
	RECT CardRect;
	RECT ClientRect;
	std::random_device rd;
	std::default_random_engine rng = std::default_random_engine(rd()); // default_random_engine have to be seeded with a unique seed on each pass 
	                                                                   // so that numbers are different each time program is ran
	//std::mt19937 rng = std::mt19937(rd());                           // mt19937 is higher quality generator, default_random_engine is for lightweight use
};