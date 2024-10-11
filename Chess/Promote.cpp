#include "Promote.h" 

Promote::Promote(string bgURL) {
	if (!this->background.loadFromFile(bgURL))
	{
		cout << "** Error: Cannot load the image! \n";
	}

	///this->startButton4.setTexture(this->button, false);
	///->startButton4.setPosition(140, 800);

	this->queenButton.setTextureRect(sf::IntRect(10, 7, 76.5, 78.4));
	this->queenButton.setPosition(10, 7);


	this->knightButton.setTextureRect(sf::IntRect(317.6, 10, 72.4, 80));
	this->knightButton.setPosition(317.6, 10);


	this->rookButton.setTextureRect(sf::IntRect(122.4, 7, 60, 83));
	this->rookButton.setPosition(122.4, 7);

	this->bishopButton.setTextureRect(sf::IntRect(218.3, 7, 70.6, 78.4));
	this->bishopButton.setPosition(218.3, 7);
	this->started = false;


	// click button sound
	if (!this->clickSoundBuffer.loadFromFile("click_sound.wav"))
	{
		cout << "** Error: Cannot load the sound file! \n";
	}
	this->clickSound.setBuffer(this->clickSoundBuffer);
}

void Promote::draw(sf::RenderTarget& target, sf::RenderStates states) const {

	sf::Sprite menu;
	menu.setTexture(this->background);

	target.draw(menu);
	target.draw(this->queenButton);
	target.draw(this->rookButton);
	target.draw(this->knightButton);
	target.draw(this->bishopButton);
	//std::cout << "Passed draw menu!" << endl;
}

sf::FloatRect Promote::queenBounds() {
	return this->queenButton.getGlobalBounds();
}

//------
sf::FloatRect Promote::rookBounds() {
	return this->rookButton.getGlobalBounds();
}

sf::FloatRect Promote::knightBounds() {
	return this->knightButton.getGlobalBounds();
}

sf::FloatRect Promote::bishopBounds() {
	return this->bishopButton.getGlobalBounds();
}

void Promote::changeState() {
	this->started = true;
}

bool Promote::getState() {
	return this->started;
}
void Promote::playClickSound()
{
	this->clickSound.play();
}