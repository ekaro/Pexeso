#pragma once
#include <windows.h>
#include <vector>
#include <random>
#include <tchar.h>
#include <string> 

class Deck
{
private:
	class Card
	{
	public:
		Card() = default;
		friend class Deck;
		void DrawCard(const HDC& hdc, int CardWidth, int CardHeight) const;
		RECT GetRect() const;		
	private:				
		COLORREF Color;
		int Left;
		int Top;
		int CardWidth;
		int CardHeight;	
		bool Exposed = false;
		int Number;
	};
public:
	Deck(int ButtonOffset);
	void NewGame();
	void DrawDeck(const HDC& hdc, const HWND& hWnd);    // drawing of every card in the deck
	void ResizeDeck(const HWND& hWnd);		   // resizing of deck according to current size of the window		
	void DrawNum(const HDC& hdc, const Card& card);
	void DrawTurns(const HDC& hdc, const HWND& hWnd);
	void CompareCards(const HWND& hWnd, int Card);
	int GetCardIndex(const HWND& hWnd, int x, int y) const;   //returns index of card that has been clicked	
	void Clicked(const HWND& hWnd, int Card);
	void ResizeText(const HWND& hWnd);
private:
	static constexpr COLORREF Green = (RGB(0, 255, 0));
	static constexpr COLORREF Blue = (RGB(0, 0, 255));
	RECT TurnsRect = { 330, 5, 430, 75};
	static constexpr TCHAR TurnsMsg[] = _T("Turns:");
	static constexpr int columns = 5;
	static constexpr int rows = 4;
	std::vector<Card> Cards;   // vector of all cards in the deck
	int CardNums[20];
	int offset;
	int FirstCard;
	int SecondCard;
	int State;
	int Turns;	
	int FontHeight = 70;
	RECT CardRect;
	HFONT CardFont;
	std::wstring CardNumber;
	std::wstring TurnsNumber;
	std::random_device rd;
	std::default_random_engine rng = std::default_random_engine(rd()); // default_random_engine have to be seeded with a unique seed on each pass 
	                                                                   // so that numbers are different each time program is ran
	//std::mt19937 rng = std::mt19937(rd());                           // mt19937 is higher quality generator, default_random_engine is for lightweight use
};