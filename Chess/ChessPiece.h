#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class ChessPiece : public sf::Drawable
{
public:
	ChessPiece(string textureURL, int x, int y, bool isBlack,string name);
	ChessPiece();
	void killed();
	bool isBlackSide();
	sf::FloatRect pieceBounds();

	void setPiece(string textureURL, int x, int y, bool isBlack, string name);
	void setPosition(int x, int y);
	int getPosition_x();
	int getPosition_y();
	string getName();

	//push-back:
	void addMove(int x);
	void addKill(int x);

	void removeMove(int i);
	void removeKill(int i);
	void deleteMove(vector<int>::iterator it);

	//get move & get Kill:
	vector<int> getMove();
	vector<int> getKill();
	vector<int> getGuard();

	//set move & set kill:
	void setMove(vector<int> v);
	void setKill(vector<int> v);

	//clear all:
	void clear();


	//virtual method
	virtual void possibleMove(vector<int> bCoords, vector<int> wCoords)=0; //Get possible move (include: move, kill, guard - hidden)
	void clearUnableMove(vector<int> temp);
	void setIsMove(bool x);
	bool getIsMove();
	void moveCal(vector<int> bCoords, vector<int> wCoords, int x2, int y2, bool& flag); //move calculator

	void setNumMove(int);
	int getNumMove();
protected:
	//var
	int x;
	int y;
	bool isBlack;
	vector<int> move;
	vector<int> kill;
	vector<int> guard;

	//method
	bool isOutside(int x, int y); // check if this coordinate is outside the board
	bool inside(int x, int y, vector<int> v); // check if piece is inside vector v
	//support:
	

private:
	sf::Texture pieceTexture;
	sf::Sprite pieceSprite;
	string name;
	bool isMove = false;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	int numMove_;
};
