#pragma once
#include <string>
#include <vector>

#include "glm/common.hpp"

#include "fmod/fmod.hpp"
#include "fmod/fmod_errors.h"

//studio
#include "FmodStudio/fmod_studio.hpp"
#include "FmodStudio/fmod_studio_common.h"

class SoundManager {
public:

	/*
	initializes the sound manager
	defaultFilePath is the default path to a sound, for example ./Assets/Sounds
	numChannels is the number of channels you want to allocate to FMOD core
	*/
	static void init(const std::string& defaultFilePath, unsigned numChannels = 512);

	/*
	A setter for the default path
	*/
	static void setDefaultPath(const std::string& defaultFilePath) { _soundPath = defaultFilePath; }

	/*
	updates the sound manager, should be called every frame
	*/
	static void update();

	/*
	loads a 2D sound with the relative file path
	returns the index at which the sound is stored in the sounds vector
	*/
	static unsigned load2DSound(const std::string& relativeFilePath);

	/*
	creates a channel group, returns the index at which it was created
	*/
	static unsigned createChannelGroup(const std::string& groupName);

	/*
	plays a 2d sound at the respective indicies
	*/
	static unsigned play2DSound(unsigned soundIndex, unsigned groupIndex);

	//set num times to loop, -1 for infinite
	static void loopSound(unsigned index, int loopCount);
	/*
	a function used to check the result of an FMOD opertation
	whereError is a parameter used to described where the error occured,
	so that when its printed in console you know where to look
	*/
	static void checkFmodErrors(FMOD_RESULT& result, const std::string& whereError);

	//The FMOD system, the main thing that does a lot of the work for you
	static FMOD::System* _system;

	static std::vector<FMOD::Sound*> _sounds;
	static std::vector<FMOD::ChannelGroup*>_channelGroups;
	static std::vector<FMOD::Channel*>_channels;
private:
	static std::string _soundPath;
	static bool _initialized;
	static FMOD_RESULT _result;

};

class Sound2D {
public:
	Sound2D(const std::string& path, const std::string& groupName);

	void play();

	void setSoundHandle(unsigned soundHandle) { _sound = soundHandle; }
	void setGroupHandle(unsigned groupHandle) { _group = groupHandle; }

	unsigned getSoundHandle() const { return _sound; }
	unsigned getGroupHandle() const { return _group; }

	void stop();

	bool isPlaying();
	void setLoopCount(int loopcount);

private:
	unsigned _sound = 0, _group = 0, _chanel = 0;
};