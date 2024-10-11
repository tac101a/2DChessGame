#include "Menu.h" 

Menu::Menu(string bgURL){
	if (!this->background.loadFromFile(bgURL))
	{
		cout << "** Error: Cannot load the image! \n";
	}
	
	this->soundButton.setTextureRect(sf::IntRect(51.9, 391.4, 205.2, 64.6));
	this->soundButton.setPosition(51.9, 391.4);
	

	this->PvPButton.setTextureRect(sf::IntRect(51.9, 206.9, 205.2, 63.8));
	this->PvPButton.setPosition(51.9, 206.9);
	

	this->PvCButton.setTextureRect(sf::IntRect(51.9, 296.3, 205.2, 63.8));
	this->PvCButton.setPosition(51.9, 296.3);

	this->exitButton.setTextureRect(sf::IntRect(51.9, 487.7, 205.2, 63.8));
	this->exitButton.setPosition(51.9, 487.7);

	this->started = false;
}

void Menu::draw(sf::RenderTarget&target, sf::RenderStates states) const{
	
	sf::Sprite menu;
	menu.setTexture(this->background);

	target.draw(menu);
	target.draw(this->soundButton);
	target.draw(this->PvPButton);
	target.draw(this->PvCButton);
	target.draw(this->exitButton);
	//std::cout << "Passed draw menu!" << endl;
}

sf::FloatRect Menu::PvPBounds() {
	return this->PvPButton.getGlobalBounds();
}

//------
sf::FloatRect Menu::PvCBounds() {
	return this->PvCButton.getGlobalBounds();
}

sf::FloatRect Menu::soundBounds() {
	return this->soundButton.getGlobalBounds();
}

sf::FloatRect Menu::exitBounds() {
	return this->exitButton.getGlobalBounds();
}

void Menu::changeState() {
	this->started =	!this->started;
}

bool Menu::getState() {
	return this->started;
}
//void Menu::playClickSound()
//{
//	this->clickSound.play();
//}