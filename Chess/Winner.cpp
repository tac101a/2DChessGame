#include "Winner.h" 

Winner::Winner(string bgURL) {
	if (!this->background.loadFromFile(bgURL))
	{
		cout << "** Error: Cannot load the image! \n";
	}

	///this->startButton4.setTexture(this->button, false);
	///->startButton4.setPosition(140, 800);

	this->replayButton.setTextureRect(sf::IntRect(362.9, 304.6, 267.7, 97.4));
	this->replayButton.setPosition(362.9, 304.6);


	this->exitButton.setTextureRect(sf::IntRect(362.9, 427.7, 267.7, 97.4));
	this->exitButton.setPosition(362.9, 427.7);


	// click button sound
	/*if (!this->clickSoundBuffer.loadFromFile("click_sound.wav"))
	{
		cout << "** Error: Cannot load the sound file! \n";
	}
	this->clickSound.setBuffer(this->clickSoundBuffer);*/
}

void Winner::draw(sf::RenderTarget& target, sf::RenderStates states) const {

	sf::Sprite menu;
	menu.setTexture(this->background);

	target.draw(menu);
	target.draw(this->replayButton);
	target.draw(this->exitButton);
	//std::cout << "Passed draw menu!" << endl;
}


//------
sf::FloatRect Winner::replayBounds() {
	return this->replayButton.getGlobalBounds();
}

sf::FloatRect Winner::exitBounds() {
	return this->exitButton.getGlobalBounds();
}


//void Menu::changeState() {
//	this->started = !this->started;
//}
//
//bool Menu::getState() {
//	return this->started;
//}
//void Menu::playClickSound()
//{
//	this->clickSound.play();
//}