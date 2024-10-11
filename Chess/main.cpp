#include<SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<iostream>
#include"GameHandler.h"
#include"Menu.h"
#include"WindowHandler.h"
using namespace std;

int Board::checkMate_ = 0;

int main() {
	char name[20] = "stockfish.exe";
	Connector::ConnectToEngine(name);
	WindowHandler windowHandler;
	windowHandler.windowProcess();
	Connector::CloseConnection();
}
