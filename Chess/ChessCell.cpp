#include "ChessCell.h"

ChessCell::ChessCell(){
	this->chessCell;
	this->chessPiece = nullptr;
	this->chessCell.setOutlineColor(sf::Color::Black);
	this->chessCell.setOutlineThickness(2);
}
void ChessCell::setPosition(sf::Vector2f vct) {
	this->chessCell.setPosition(vct);
}
void ChessCell::setSize(sf::Vector2f vct) {
	this->chessCell.setSize(vct);
}
void ChessCell:: setColor(sf::Color c) {
	this->chessCell.setFillColor(c);

}
void ChessCell::setPiece(ChessPiece* piece) {
	this->chessPiece = piece;
}

void ChessCell::draw(sf::RenderTarget& target, sf::RenderStates states) const {

	target.draw(this->chessCell);
	if (this->chessPiece)
	{
		target.draw(*(this->chessPiece));
	}
}

sf::FloatRect ChessCell::getBounds() {
	return this->chessCell.getGlobalBounds();
}

void ChessCell::cellPressed(){
	
}



ChessPiece* ChessCell::getPiece(){
	return this->chessPiece;
}

sf::Color ChessCell::getColor() {
	return this->chessCell.getFillColor();
}

