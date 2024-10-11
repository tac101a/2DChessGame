#pragma once
#include<iostream>
#include<SFML/Audio.hpp>
using namespace std;

class Sound {
private:
	sf::SoundBuffer soundTrackBuffer;
	sf::Sound soundTrack;

	sf::SoundBuffer soundStartBuffer;
	sf::Sound soundStart;

	sf::Sound moveSound;
	sf::SoundBuffer moveSoundBuffer;

	sf::Sound killSound;
	sf::SoundBuffer killSoundBuffer;

	sf::Sound checkedSound;
	sf::SoundBuffer checkedSoundBuffer;

	sf::Sound winSound;
	sf::SoundBuffer winSoundBuffer;

	sf::Sound replaySound;
	sf::SoundBuffer replaySoundBuffer;

	sf::Sound castleSound;
	sf::SoundBuffer castleSoundBuffer;

	sf::Sound promoteSound;
	sf::SoundBuffer promoteSoundBuffer;

public:
	//Sound(string soundTrackBuffer, string moveSoundBuffer, string killSoundBuffer, string checkedSoundBuffer, string winSoundBuffer, string replaySoundBuffer, string promoteSoundBuffer, string castleSoundBuffer);
	~Sound();
	Sound();
	void playMoveSound();
	void playKillSound();
	void playCheckedSound();
	void playSoundTrack();
	void playReplaySound();
	void playPromoteSound();
	void playCastleSound();
	void playWinSound();
	void playSoundStart();
	void stopSoundTrack();
	void stopSoundStart();
	void muteAllSound();
	void onAllSound();
	void stopWinSound();
	void setVolume(float x);
	float getVolume();
};