#pragma once
#include <array>
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Menu.h"
//pieces including
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Connector.h"

using namespace std;

class GameHandler : public sf::Drawable
{
public:
	GameHandler(sf::Color c1, sf::Color c2);
	GameHandler() {};
	~GameHandler();

	void setVolume(float x);
	void gameProcess(sf::Vector2f mouse);
	void undoProcess();
	void redoProcess();
	sf::FloatRect undoBounds();
	sf::FloatRect redoBounds();
	sf::FloatRect backBounds();
	bool replayGame();
	void resetGame();
	void resetSave();

	void ComputerProcess(sf::Vector2f);
private:
	Board chessBoard;

	Pawn* whitePawn;
	Pawn* blackPawn;

	Rook* whiteRook;
	Rook* blackRook;

	Knight* blackKnight;
	Knight* whiteKnight;

	Bishop* blackBishop;
	Bishop* whiteBishop;

	Queen* blackQueen;
	Queen* whiteQueen;

	King* blackKing;
	King* whiteKing;

	bool isWhiteTurn; 

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Texture texture;
	sf::Sprite sprite;


	sf::Sprite buttonUndo;
	sf::Sprite buttonRedo;
	sf::Sprite buttonBack;
	
};

