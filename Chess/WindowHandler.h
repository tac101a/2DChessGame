#pragma once
#include<SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<iostream>
#include"GameHandler.h"
#include"Menu.h"
#include"Winner.h"
#include"Sound.h"
using namespace std;

class WindowHandler {
private:
	sf::RenderWindow* window;
	Menu* menu;
	GameHandler* gameHandler;
	Winner* winner;
	Sound* sound;
	bool block = false;
	bool pvp = false;
	bool pvc = false;
	bool _sound = true;
public:
	WindowHandler();
	~WindowHandler();
	void windowProcess();
	void backProcess();
	void winProcess();
	void soundSetting();
};