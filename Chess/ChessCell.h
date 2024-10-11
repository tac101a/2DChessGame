#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "ChessPiece.h"

class ChessCell : public sf::Drawable
{
public:
	ChessCell();

	void setPosition(sf::Vector2f vct);
	void setSize(sf::Vector2f vct);
	void setColor(sf::Color c);
	void setPiece(ChessPiece* piece);

	sf::Color getColor();
	sf::FloatRect getBounds();
	ChessPiece* getPiece();

	void cellPressed();


private:
	ChessPiece* chessPiece;
	sf::RectangleShape chessCell;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
