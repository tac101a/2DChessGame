#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>

using namespace std;

class Winner : public sf::Drawable
{
public:
	Winner(string);
	sf::FloatRect replayBounds();
	sf::FloatRect exitBounds();

	//void playClickSound();
private:
	sf::Texture background;

	sf::Sprite replayButton;
	sf::Sprite exitButton;
	//bool started;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	//sf::SoundBuffer clickSoundBuffer;
	//sf::Sound clickSound;
};
