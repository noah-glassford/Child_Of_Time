#include "AudioManager.h"

void Soundfunctions::PlaySingleSound(std::string Filename)
{
	std::string SoundString = "play " + Filename + " from 0";
	mciSendString(SoundString.c_str(), NULL, 0, NULL);
}

void Soundfunctions::LoopSound(std::string Filename)
{
	std::string SoundString = "play " + Filename + " repeat";
	mciSendString(SoundString.c_str(), NULL, 0, NULL);
}
void Soundfunctions::PauseSound(std::string Filename)
{
	std::string SoundString = "stop " + Filename;
	mciSendString(SoundString.c_str(), NULL, 0, NULL);
}

void Soundfunctions::LoadSound(std::string Filename)
{
	std::string SoundString = "open assets/Audio_Files/" + Filename + " type mpegvideo alias " + Filename;
	mciSendString(SoundString.c_str(), NULL, 0, NULL);
}

void Soundfunctions::AdjustVolume(std::string Filename, int Percentage)
{
	std::string SoundString = "setaudio " + Filename + " volume to " + std::to_string(Percentage);
	mciSendString(SoundString.c_str(), NULL, 0, NULL);
}
