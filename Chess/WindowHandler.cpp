#include "WindowHandler.h"
#include<thread>
#include<chrono>

WindowHandler::WindowHandler()
{
	this->window = new sf::RenderWindow(sf::VideoMode(960, 640), "Chess", sf::Style::Titlebar | sf::Style::Close);
	this->window->setVerticalSyncEnabled(true);
	this->menu=new Menu("../Img/Menu/Menu.png");
	this->gameHandler=new GameHandler(sf::Color{ 250,245,246 }, sf::Color{ 255,142,176 });
	this->winner = nullptr;
	//this->sound = new Sound(".. / Sound / background.wav", ".. / Sound / move.wav", ".. / Sound / kill.wav", ".. / Sound / check.wav", ".. / Sound / win.wav", ".. / Sound / replay.wav", ".. / Sound / promote.wav", ".. / Sound / castle.wav");
	this->sound = new Sound();
}

WindowHandler::~WindowHandler()
{
	delete this->window;
	delete this->gameHandler;
	delete this->menu;
	delete this->winner;
	delete this->sound;
}
void WindowHandler::backProcess(){
	pvp = false;
	pvc = false;
	this->gameHandler->resetSave();
	this->sound->stopSoundTrack();
	this->sound->playSoundStart();
	//Window resize and reposition:
	this->menu->changeState();
	sf::Vector2u size(960, 640);
	this->window->setSize(size);
	sf::Vector2i pos = this->window->getPosition();
	pos.x = 470;
	pos.y = 220;
	this->window->setPosition(pos);
	this->window->draw(*this->menu);
	this->window->display();
}

void WindowHandler::winProcess()
{
	this->sound->playWinSound();
	sf::RenderWindow window(sf::VideoMode(960, 640), "Congratulation !!!", sf::Style::Titlebar);
	window.setVerticalSyncEnabled(true);
	//sf::Texture background;
	if (Board::checkMate_ == 1) {

		this->winner = new Winner("../Img/Menu/whiteWin.png");
	}
	else if (Board::checkMate_ == -1) {
	
		this->winner = new Winner("../Img/Menu/blackWin.png");
	}

	window.draw(*this->winner);
	sf::Vector2i pos = window.getPosition();
	pos.x = 550;
	pos.y = 220;
	window.setPosition(pos);
	window.display();
	//std::this_thread::sleep_for(std::chrono::milliseconds(7000));
	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			//Close console
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			//Event handler
			if (event.type == sf::Event::MouseButtonPressed)
			{
				//Press in "Start" button	
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					sf::Vector2f mouse = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));

					if (this->winner->replayBounds().contains(mouse))
					{
						this->sound->stopWinSound();
						this->sound->playReplaySound();
						window.close();
						Board::checkMate_ = 0;
						block = true;
						std::this_thread::sleep_for(std::chrono::milliseconds(5000));
						this->sound->playSoundTrack();
						this->gameHandler->resetGame();
						this->gameHandler->gameProcess(mouse);
						this->window->draw(*this->gameHandler);
						this->window->display();
						this->window->setTitle("Replay");
						std::this_thread::sleep_for(std::chrono::milliseconds(1500));
						while (block == true) {
							bool x = this->gameHandler->replayGame();
							if (x == false) {
								block = false;
							}
							else
							{
								this->gameHandler->gameProcess(mouse);
								this->window->draw(*this->gameHandler);
								this->window->display();
								std::this_thread::sleep_for(std::chrono::milliseconds(1500));
							}
						}
						this->sound->stopSoundTrack();

						this->backProcess();
						continue;
					}
					if (this->winner->exitBounds().contains(mouse))
					{
						this->sound->stopWinSound();
						window.close();
						Board::checkMate_ = 0;

						this->menu->changeState();
						sf::Vector2u size(960, 640);
						this->window->setSize(size);
						sf::Vector2i pos = this->window->getPosition();
						pos.x = 470;
						pos.y = 220;
						this->window->setPosition(pos);

						this->gameHandler->~GameHandler();
						this->gameHandler=new GameHandler(sf::Color{ 250,245,246 }, sf::Color{ 255,142,176 });

						//this->menu->playClickSound();
						this->window->draw(*this->menu);
						this->window->display();
					}
				}
			}
		}
	}
}

void WindowHandler::windowProcess()
{
	
	this->sound->playSoundStart();

	this->window->draw(*this->menu);

	this->window->display();

	while (this->window->isOpen())
	{
		sf::Event event;

		while (this->window->pollEvent(event))
		{
			//Close console
			if (event.type == sf::Event::Closed)
			{
				this->window->close();
			}

			//Event handler
			if (event.type == sf::Event::MouseButtonPressed)
			{
				//Press in "Start" button	
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					sf::Vector2f mouse = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));

					if (this->menu->PvPBounds().contains(mouse) && !this->menu->getState())
					{
						pvp = true;
						this->sound->stopSoundStart();
						this->sound->playSoundTrack();

						this->gameHandler->resetGame();
						this->menu->changeState();
						//Window resize and reposition:
						sf::Vector2u size(1200, 1000);
						this->window->setSize(size);
						sf::Vector2i pos = this->window->getPosition();
						pos.x = 473;
						pos.y = 0;
						this->window->setPosition(pos);

						//Game screen appeared:
						this->window->draw(*this->gameHandler);
						this->window->display();
					}

					if (this->menu->getState() && pvp == true)
					{
						this->gameHandler->gameProcess(mouse);
						this->window->draw(*this->gameHandler);
						this->window->display();

						if (gameHandler->undoBounds().contains(mouse)) {
							gameHandler->undoProcess();
						}
						if (gameHandler->redoBounds().contains(mouse)) {
							gameHandler->redoProcess();
						}
						if (gameHandler->backBounds().contains(mouse)) {
							this->backProcess();
						}
						if (Board::checkMate_ != 0) {
							this->sound->stopSoundTrack();
							this->winProcess();
						}
						continue;
					}

					//press in PvP button
					if (this->menu->soundBounds().contains(mouse) && !this->menu->getState())
					{
						this->soundSetting();
					}

					//Press in PvC button
			/*		if (event.mouseButton.button == sf::Mouse::Left)
					{
						sf::Vector2f mouse = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));*/

					if (this->menu->PvCBounds().contains(mouse) && !this->menu->getState())
					{
						pvc = true;
						this->sound->stopSoundStart();
						this->sound->playSoundTrack();

						this->gameHandler->resetGame();
						this->menu->changeState();
						//Window resize and reposition:
						sf::Vector2u size(1200, 1000);
						this->window->setSize(size);
						sf::Vector2i pos = this->window->getPosition();
						pos.x = 473;
						pos.y = 0;
						this->window->setPosition(pos);

						//Game screen appeared:
						this->window->draw(*this->gameHandler);
						this->window->display();
					}

					if (this->menu->getState() && pvc == true)
					{
						this->gameHandler->ComputerProcess(mouse);
						this->window->draw(*this->gameHandler);
						this->window->display();
						if (gameHandler->undoBounds().contains(mouse)) {
							gameHandler->undoProcess();
						}
						if (gameHandler->redoBounds().contains(mouse)) {
							gameHandler->redoProcess();
						}
						if (gameHandler->backBounds().contains(mouse)) {
							this->backProcess();
						}

						if (Board::checkMate_ != 0) {
							this->sound->stopSoundTrack();
							this->winProcess();
						}
						continue;
					}



					if (this->menu->exitBounds().contains(mouse))
					{
						this->window->close();
					}
				}
			}
		}

	}
}
void WindowHandler::soundSetting() {
	sf::RenderWindow window(sf::VideoMode(400, 200), "Volume Setting");
	sf::Event event;

	// Tạo một thanh trượt để điều chỉnh âm lượng
	sf::RectangleShape volumeSlider(sf::Vector2f(200, 20));
	volumeSlider.setFillColor(sf::Color{ 255,142,176 });
	volumeSlider.setPosition(40, 120);

	float volume = this->sound->getVolume();
	// Tạo một ô vuông để di chuyển trên thanh trượt
	sf::RectangleShape thumb(sf::Vector2f(20, 40));
	thumb.setFillColor(sf::Color{ 250, 245, 246 });
	thumb.setPosition(volume / 100 * 180 + 40, 110);

	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("../Sound/soundSetting.png"))
	{
		// Xử lý khi không thể tải ảnh nền
		cout << "** Error: Cannot load the sound file! \n";
	}
	sf::Sprite background;
	background.setTexture(backgroundTexture);

	window.draw(background);
	window.display();

	bool isDragging = false; // Biến kiểm tra trạng thái kéo thả
	float offsetX = 0.0f;    // Khoảng cách giữa chuột và vị trí ban đầu của ô vuông

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();


			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

					if (thumb.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
					{
						isDragging = true;
						offsetX = mousePosition.x - thumb.getPosition().x;
					}
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					isDragging = false;
				}
			}
		}

		if (isDragging)
		{
			sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
			float thumbX = mousePosition.x - offsetX;

			// Giới hạn vị trí của ô vuông trong thanh trượt
			if (thumbX < 40)
				thumbX = 40;
			else if (thumbX > 220)
				thumbX = 220;

			thumb.setPosition(thumbX, 110);

			// Cập nhật âm lượng dựa trên vị trí của ô vuông
			float relativeX = thumbX - 40;
			float volumeRatio = relativeX / 180;
			volume = volumeRatio * 100;
			this->sound->setVolume(volume);
			this->gameHandler->setVolume(volume);
		}
		window.clear();
		window.draw(background);
		window.draw(volumeSlider);
		window.draw(thumb);

		window.display();
	}
}

