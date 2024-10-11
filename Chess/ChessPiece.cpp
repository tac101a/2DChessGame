#include "ChessPiece.h"

//constructor:
ChessPiece::ChessPiece(string textureURL, int x, int y, bool isBlack,string name) {
	if (!this->pieceTexture.loadFromFile(textureURL))
	{
		std::cout << "**Error: Cannot load image from file! \n";
		exit(-1);
	}
	this->pieceSprite.setTexture(this->pieceTexture);
	this->x = x;
	this->y = y;
	this->isBlack = isBlack;
	this->name = name;
}

ChessPiece::ChessPiece() {
	this->isBlack = false;
	this->x = -1;
	this->y = -1;
	this->pieceSprite;
	this->pieceTexture;
	this->name = "";
}


void ChessPiece::killed() {
	//dont know what to use yet
}


//set texture and attribute for a piece
void ChessPiece::setPiece(string textureURL, int x, int y, bool isBlack,string name) {
	if (!this->pieceTexture.loadFromFile(textureURL))
	{
		std::cout << "**Error: Cannot load image from file! \n";
		exit(-1);
	}
	this->pieceSprite.setTexture(this->pieceTexture);
	this->x = x;
	this->y = y;
	this->isBlack = isBlack;
	this->pieceSprite.setPosition(sf::Vector2f(y * 96.f + 96.f, x * 64.f + 64.f));
	this->name = name;
}

sf::FloatRect ChessPiece::pieceBounds() {
	return this->pieceSprite.getGlobalBounds();
}


bool ChessPiece::isBlackSide() {
	return this->isBlack;
}

string ChessPiece::getName() {
	return this->name;
}

void ChessPiece::setPosition(int x, int y) {
	this->x = x;
	this->y = y;
	this->pieceSprite.setPosition(sf::Vector2f(y * 96.f+96.f, x * 64.f+64.f));
}


//--------------PROTECTED METHOD ------------

bool ChessPiece::isOutside(int x, int y) {
	if (x >= 8 || x < 0)
	{
		return true; 
	}
	if (y >= 8 || y < 0)
	{
		return true;
	}
	return false;
}

bool ChessPiece::inside(int x, int y, vector<int> v) {
	int temp = x * 8 + y;
	
	for (int i = 0; i < v.size(); i++)
	{
		if (temp == v[i])
		{
			return true;
		}
	}
	return false;
}


void ChessPiece::moveCal(vector<int> bCoords, vector<int> wCoords, int x2, int y2, bool &flag) {
	if (flag) return;
	if (isOutside(x2, y2)) return;
	// check inside wCoords
	if (this->inside(x2, y2, wCoords))
	{
		if (this->isBlack) this->kill.push_back(x2 * 8 + y2);
		else this->guard.push_back(x2 * 8 + y2);
		flag = true;
	}
	// check inside bCoords
	else if (this->inside(x2, y2, bCoords))
	{
		if (this->isBlack) this->guard.push_back(x2 * 8 + y2);
		else this->kill.push_back(x2 * 8 + y2);
		flag = true;
	}
	else this->move.push_back(x2 * 8 + y2);
}



//--------Vector Supporting method------
void ChessPiece::addMove(int x) {
	this->move.push_back(x);
}
void ChessPiece::addKill(int x) {
	this->kill.push_back(x);
}
void ChessPiece::removeMove(int i) {
	this->move.erase(this->move.begin() + i);
}
void ChessPiece::removeKill(int i) {
	this->kill.erase(this->kill.begin() + i);
}

vector<int> ChessPiece::getMove() {
	return this->move;
}
vector<int> ChessPiece::getKill() {
	return this->kill;
}
vector<int> ChessPiece::getGuard() {
	return this->guard;
}

int ChessPiece::getPosition_x() {
	return this->x;
}
int ChessPiece::getPosition_y() {
	return this->y;
}
void ChessPiece::setIsMove(bool x) {
	this->isMove = x;
}
bool ChessPiece::getIsMove() {
	return this->isMove;
}

void ChessPiece::setMove(vector<int> v) {
	this->move = v;
}
void ChessPiece::setKill(vector<int> v) {
	this->kill = v;
}

void ChessPiece::clear() {
	this->move.clear();
	this->kill.clear();
	this->guard.clear();
}

void ChessPiece::deleteMove(vector<int>::iterator it) {
	this->move.erase(it);
}


void ChessPiece::setNumMove(int x) {
	this->numMove_ = x;
}


int ChessPiece::getNumMove() { return this->numMove_; }

//-------------VIRTUAL METHOD----------

//draw method: display texture
void ChessPiece::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(this->pieceSprite);
}


void ChessPiece::clearUnableMove(vector<int> temp) {

	for (int i = 0; i < this->move.size(); i++)
	{
		vector<int>::iterator it = std::find(temp.begin(), temp.end(), this->move[i]);
		if (it == temp.end()) continue;//safe move
		//cout << "Found unsafe move \n";

		this->move.erase(this->move.begin() + i);//delete unsafe move
		i--;
	}

	for (int i = 0; i < this->kill.size(); i++)
	{
		vector<int>::iterator it = std::find(temp.begin(), temp.end(), this->kill[i]);
		if (it == temp.end()) continue;//safe move
		//cout << "Found unsafe move \n";
		this->kill.erase(this->kill.begin() + i);//delete unsafe move
		i--;
	}
}