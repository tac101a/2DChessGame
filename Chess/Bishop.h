#pragma once
#include "ChessPiece.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>

using namespace std;

class Bishop : public ChessPiece
{
public:
	Bishop();
	void possibleMove(vector<int> bCoords, vector<int> wCoords); //Get possible move


private:
};
