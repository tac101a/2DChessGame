#include "Sound.h"

//Sound::Sound(string soundTrackBuffer, string moveSoundBuffer, string killSoundBuffer, string checkedSoundBuffer, string winSoundBuffer, string replaySoundBuffer,string promoteSoundBuffer,string castleSoundBuffer)
//{
//	
//	if (!this->soundTrackBuffer.loadFromFile(soundTrackBuffer)) 
//		cout << "** Error: Cannot load the sound file! \n";
//
//	this->soundTrack.setBuffer(this->soundTrackBuffer);
//
//	if (!this->moveSoundBuffer.loadFromFile(moveSoundBuffer)) 
//		cout << "** Error: Cannot load the sound file! \n";
//	this->moveSound.setBuffer(this->moveSoundBuffer);
//
//	if (!this->killSoundBuffer.loadFromFile(killSoundBuffer))
//		cout << "** Error: Cannot load the sound file! \n";
//	this->killSound.setBuffer(this->killSoundBuffer);
//
//	if (!this->checkedSoundBuffer.loadFromFile(checkedSoundBuffer))
//		cout << "** Error: Cannot load the sound file! \n";
//	this->checkedSound.setBuffer(this->checkedSoundBuffer);
//
//	if (!this->winSoundBuffer.loadFromFile(winSoundBuffer))
//		cout << "** Error: Cannot load the sound file! \n";
//	this->winSound.setBuffer(this->winSoundBuffer);
//
//	if (!this->replaySoundBuffer.loadFromFile(replaySoundBuffer))
//		cout << "** Error: Cannot load the sound file! \n";
//	this->replaySound.setBuffer(this->replaySoundBuffer);
//
//	if (!this->promoteSoundBuffer.loadFromFile(promoteSoundBuffer))
//		cout << "** Error: Cannot load the sound file! \n";
//	this->promoteSound.setBuffer(this->promoteSoundBuffer);
//
//	if (!this->castleSoundBuffer.loadFromFile(castleSoundBuffer))
//		cout << "** Error: Cannot load the sound file! \n";
//	this->castleSound.setBuffer(this->castleSoundBuffer);
//}

Sound::~Sound()
{
}

Sound::Sound()
{
	if (!this->soundTrackBuffer.loadFromFile("../Sound/background.wav"))
		cout << "** Error: Cannot load the sound file! \n";

	this->soundTrack.setBuffer(this->soundTrackBuffer);

	if (!this->soundStartBuffer.loadFromFile("../Sound/start.wav"))
		cout << "** Error: Cannot load the sound file! \n";

	this->soundStart.setBuffer(this->soundStartBuffer);

	if (!this->moveSoundBuffer.loadFromFile("../Sound/move.wav"))
		cout << "** Error: Cannot load the sound file! \n";
	this->moveSound.setBuffer(this->moveSoundBuffer);

	if (!this->killSoundBuffer.loadFromFile("../Sound/kill.wav"))
		cout << "** Error: Cannot load the sound file! \n";
	this->killSound.setBuffer(this->killSoundBuffer);

	if (!this->checkedSoundBuffer.loadFromFile("../Sound/check.wav"))
		cout << "** Error: Cannot load the sound file! \n";
	this->checkedSound.setBuffer(this->checkedSoundBuffer);

	if (!this->winSoundBuffer.loadFromFile("../Sound/win.wav"))
		cout << "** Error: Cannot load the sound file! \n";
	this->winSound.setBuffer(this->winSoundBuffer);

	if (!this->replaySoundBuffer.loadFromFile("../Sound/replay.wav"))
		cout << "** Error: Cannot load the sound file! \n";
	this->replaySound.setBuffer(this->replaySoundBuffer);

	if (!this->promoteSoundBuffer.loadFromFile("../Sound/promote.wav"))
		cout << "** Error: Cannot load the sound file! \n";
	this->promoteSound.setBuffer(this->promoteSoundBuffer);

	if (!this->castleSoundBuffer.loadFromFile("../Sound/castle.wav"))
		cout << "** Error: Cannot load the sound file! \n";
	this->castleSound.setBuffer(this->castleSoundBuffer);
}

void Sound::playMoveSound()
{
	this->moveSound.play();
}

void Sound::playKillSound()
{
	this->killSound.play();
}

void Sound::playCheckedSound()
{
	this->checkedSound.play();

}
void Sound::playSoundStart() {
	this->soundTrack.setLoop(100);
	this->soundStart.play();
}
void Sound::playSoundTrack()
{
	this->soundTrack.setVolume(10);
	this->soundTrack.setLoop(100);
	this->soundTrack.play();
}

void Sound::playReplaySound()
{
	this->replaySound.play();
}

void Sound::playPromoteSound()
{
	this->promoteSound.play();
}

void Sound::playCastleSound()
{
	this->castleSound.play();
}

void Sound::playWinSound()
{
	this->winSound.play();
}

void Sound::stopSoundTrack()
{
	this->soundTrack.stop();
}
void Sound::stopSoundStart()
{
	this->soundStart.stop();
}
void Sound::stopWinSound()
{
	this->winSound.stop();
}
void Sound::muteAllSound() {
	this->soundStart.setVolume(0);
	this->soundTrack.setVolume(0); 
	this->moveSound.setVolume(0);
	this->killSound.setVolume(0);
	this->checkedSound.setVolume(0);
	this->castleSound.setVolume(0);
	this->promoteSound.setVolume(0);
	this->winSound.setVolume(0);
}

void Sound::onAllSound()
{
	this->soundStart.setVolume(100);
	this->soundTrack.setVolume(10);
	this->moveSound.setVolume(100);
	this->killSound.setVolume(100);
	this->checkedSound.setVolume(100);
	this->castleSound.setVolume(100);
	this->promoteSound.setVolume(100);
	this->winSound.setVolume(100);
}
void Sound::setVolume(float x) {
	this->soundStart.setVolume(x);
	this->soundTrack.setVolume(x / 10);
	this->moveSound.setVolume(x);
	this->killSound.setVolume(x);
	this->checkedSound.setVolume(x);
	this->castleSound.setVolume(x);
	this->promoteSound.setVolume(x);
	this->winSound.setVolume(x);
}
float Sound::getVolume() {
	return this->soundStart.getVolume();
}