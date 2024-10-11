#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>

using namespace std;

class Menu : public sf::Drawable
{
public:
	Menu(string);
	Menu() {};
	sf::FloatRect soundBounds();
	sf::FloatRect PvPBounds();
	sf::FloatRect PvCBounds();
	sf::FloatRect exitBounds();

	void changeState();
	bool getState();
	
	//void playClickSound();
private:
	sf::Texture background;
	//sf::Texture button;
	sf::Sprite soundButton;
	sf::Sprite PvPButton;
	sf::Sprite PvCButton;
	sf::Sprite exitButton;
	bool started;

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	//sf::SoundBuffer clickSoundBuffer;
	//sf::Sound clickSound;
};
