#include "GameHandler.h"

//Game init
GameHandler::GameHandler(sf::Color c1 = sf::Color{ 250,245,246 }, sf::Color c2 = sf::Color{ 255,142,176 }) : chessBoard(c1, c2) {
	std::cout << "Started successfully! \n";

	// Load background
	this->texture.loadFromFile("../Img/Menu/BoardBg.png");
	sf::Vector2u size = texture.getSize();
	sprite.setTexture(texture);  //This is where you add an & to designate texture as a pointer
	sprite.setPosition(0.f, 0.f);

	this->buttonUndo.setTextureRect(sf::IntRect(881.9, 219.6, 66, 66));
	this->buttonUndo.setPosition(881.9, 219.6);
	 
	this->buttonRedo.setTextureRect(sf::IntRect(881.9, 359.7, 66, 66));
	this->buttonRedo.setPosition(881.9, 359.7);

	this->buttonBack.setTextureRect(sf::IntRect(3.3, 13, 92.7, 39.3));
	this->buttonBack.setPosition(3.3, 13);

	//set chess board
	this->chessBoard.display();

	//set pawns
	this->blackPawn = new(std::nothrow) Pawn[8];
	if (!this->blackPawn)
	{
		throw "Allocating failed! \n";
		exit(-1);
	}
	this->whitePawn = new(std::nothrow) Pawn[8];
	if (!this->whitePawn)
	{
		throw "Allocating failed! \n";
		exit(-1);
	}
	for (int i = 0; i < 8; i++)
	{
		this->blackPawn[i].setPiece("../Img/Texture/blackPawnn.png", 1, i, true, "Pawn");
		this->chessBoard.setPiece(1, i, &blackPawn[i]);
		this->whitePawn[i].setPiece("../Img/Texture/whitePawnn.png", 6, i, false, "Pawn");
		this->chessBoard.setPiece(6, i, &whitePawn[i]);
	}

	//set rooks
	this->blackRook = new (std::nothrow) Rook[2];
	if (!this->blackRook)
	{
		throw "Allocating failed! \n";
		exit(-1);
	}
	this->whiteRook = new (std::nothrow) Rook[2];
	if (!this->whiteRook) {
		throw "Allocating failed! \n";
		exit(-1);
	}

	this->blackRook[0].setPiece("../Img/Texture/blackRookk.png", 0, 0, true,"Rook");
	this->chessBoard.setPiece(0, 0, &blackRook[0]);
	this->blackRook[1].setPiece("../Img/Texture/blackRookk.png", 0, 7, true, "Rook");
	this->chessBoard.setPiece(0, 7, &blackRook[1]);

	this->whiteRook[0].setPiece("../Img/Texture/whiteRookk.png", 7, 0, false, "Rook");
	this->chessBoard.setPiece(7, 0, &whiteRook[0]);
	this->whiteRook[1].setPiece("../Img/Texture/whiteRookk.png", 7, 7, false, "Rook");
	this->chessBoard.setPiece(7, 7, &whiteRook[1]);

	//set knights
	this->blackKnight = new(std::nothrow) Knight[2];
	if (!this->blackKnight)
	{
		throw "Allocating failed! \n";
		exit(-1);
	}
	this->whiteKnight = new(std::nothrow) Knight[2];
	if (!this->whiteKnight)
	{
		throw "Allocating failed! \n";
		exit(-1);
	}
	this->blackKnight[0].setPiece("../Img/Texture/blackKnightt.png", 0, 1, true, "Knight");
	this->chessBoard.setPiece(0, 1, &blackKnight[0]);

	this->blackKnight[1].setPiece("../Img/Texture/blackKnightt.png", 0, 6, true, "Knight");
	this->chessBoard.setPiece(0, 6, &blackKnight[1]);

	this->whiteKnight[0].setPiece("../Img/Texture/whiteKnightt.png", 7, 1, false, "Knight");
	this->chessBoard.setPiece(7, 1, &whiteKnight[0]);

	this->whiteKnight[1].setPiece("../Img/Texture/whiteKnightt.png", 7, 6, false, "Knight");
	this->chessBoard.setPiece(7, 6, &whiteKnight[1]);

	//set bishops
	this->blackBishop = new(std::nothrow) Bishop[2];
	if (!this->blackBishop)
	{
		throw "Allocating failed! \n";
		exit(-1);
	}
	this->whiteBishop = new(std::nothrow) Bishop[2];
	if (!this->whiteBishop)
	{
		throw "Allocating failed! \n";
		exit(-1);
	}
	this->blackBishop[0].setPiece("../Img/Texture/blackBishopp.png", 0, 2, true, "Bishop");
	this->chessBoard.setPiece(0, 2, &blackBishop[0]);

	this->blackBishop[1].setPiece("../Img/Texture/blackBishopp.png", 0, 5, true, "Bishop");
	this->chessBoard.setPiece(0, 5, &blackBishop[1]);

	this->whiteBishop[0].setPiece("../Img/Texture/whiteBishopp.png", 7, 2, false, "Bishop");
	this->chessBoard.setPiece(7, 2, &whiteBishop[0]);

	this->whiteBishop[1].setPiece("../Img/Texture/whiteBishopp.png", 7, 5, false, "Bishop");
	this->chessBoard.setPiece(7, 5, &whiteBishop[1]);

	//set queens
	this->blackQueen = new(std::nothrow) Queen;
	if (!this->blackQueen)
	{
		throw "Allocating failed! \n";
		exit(-1);
	}
	this->whiteQueen = new(std::nothrow) Queen;
	if (!this->whiteQueen)
	{
		throw "Allocating failed! \n";
		exit(-1);
	}
	this->blackQueen->setPiece("../Img/Texture/blackQueenn.png", 0, 3, true, "Queen");
	this->chessBoard.setPiece(0, 3, blackQueen);

	this->whiteQueen->setPiece("../Img/Texture/whiteQueenn.png", 7, 3, false, "Queen");
	this->chessBoard.setPiece(7, 3, whiteQueen);

	//set kings
	this->blackKing = new(std::nothrow) King;
	if (!this->blackKing)
	{
		throw "Allocating failed! \n";
		exit(-1);
	}
	this->whiteKing = new(std::nothrow) King;
	if (!this->whiteKing)
	{
		throw "Allocating failed! \n";
		exit(-1);
	}
	this->blackKing->setPiece("../Img/Texture/blackKingg.png", 0, 4, true, "King");
	this->chessBoard.setPiece(0, 4, blackKing);

	this->whiteKing->setPiece("../Img/Texture/whiteKingg.png", 7, 4, false, "King");
	this->chessBoard.setPiece(7, 4, whiteKing);


	//set turn:
	this->isWhiteTurn = true;

	//map calculate:
	this->chessBoard.mapCalculate();
	
}

//Destructor
GameHandler::~GameHandler() {
	delete[] this->blackPawn;
	delete[] this->whitePawn;
	delete[] this->blackBishop;
	delete[] this->whiteBishop;
	delete[] this->blackKnight;
	delete[] this->whiteKnight;
	delete[] this->blackRook;
	delete[] this->whiteRook;

	delete this->blackKing;
	delete this->whiteKing;
	delete this->blackQueen;
	delete this->whiteQueen;

  	this->blackPawn = nullptr;
	this->whitePawn = nullptr;
	this->blackRook = nullptr;
	this->whiteRook = nullptr;
	this->blackBishop = nullptr;
	this->whiteBishop = nullptr;
	this->blackKnight = nullptr;
	this->whiteKnight = nullptr;
	
	this->blackKing = nullptr;
	this->whiteKing = nullptr;
	this->blackQueen = nullptr;
	this->whiteQueen = nullptr;

}

//Game Process
void GameHandler:: gameProcess(sf::Vector2f mouse){
	this->chessBoard.moveHandle(mouse);
}

bool GameHandler::replayGame() {
	return this->chessBoard.replay();
}
void GameHandler::resetGame() {
	this->chessBoard.resetPlay();
}
void GameHandler::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	//target.clear(sf::Color::Black);
	target.draw(sprite);
	target.draw(chessBoard);

}
void GameHandler::undoProcess() {
	this->chessBoard.undoMove();
}
void GameHandler::redoProcess() {
	this->chessBoard.redoMove();
}
void GameHandler::resetSave() {
	this->chessBoard.resetSaveChess();
}

void GameHandler::ComputerProcess(sf::Vector2f mouse)
{
	//string temp;
	//this->chessBoard.moveHandle(mouse);
	this->chessBoard.moveHandleForComputer(mouse);
}

// button undo redo
sf::FloatRect GameHandler::undoBounds() {
	return this->buttonUndo.getGlobalBounds();
}

sf::FloatRect GameHandler::redoBounds() {
	return this->buttonRedo.getGlobalBounds();
}
sf::FloatRect GameHandler::backBounds() {
	return this->buttonBack.getGlobalBounds();
}

void GameHandler::setVolume(float x) {
	this->chessBoard.setVolume(x);
}





