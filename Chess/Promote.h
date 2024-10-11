#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>

using namespace std;

class Promote : public sf::Drawable
{
public:
	Promote(string);
	sf::FloatRect queenBounds();
	sf::FloatRect rookBounds();
	sf::FloatRect knightBounds();
	sf::FloatRect bishopBounds();

	void changeState();
	bool getState();

	void playClickSound();
private:
	sf::Texture background;
	sf::Sprite queenButton;
	sf::Sprite knightButton;
	sf::Sprite rookButton;
	sf::Sprite bishopButton;
	bool started;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::SoundBuffer clickSoundBuffer;
	sf::Sound clickSound;
};
