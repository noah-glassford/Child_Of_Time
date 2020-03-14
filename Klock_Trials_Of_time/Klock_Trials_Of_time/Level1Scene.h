#pragma once

#include "Scene.h"
#include "SoundEngine.h"

class Level1Scene : public Scene
{
public:
	Level1Scene(std::string name);

	//Each Scene will need to have a different
	//init, as each scene's content will be different
	void InitScene(float windowWidth, float windowHeight) override;
	void Update() override;
	bool createdint{ 0 }; //Counts frames since created a combat hitbox
	unsigned int tempent;
	bool tempbool{ 1 };

	//functions to make it less messy this is a huge file anyways
	void PlatformMovement();
	void GeneralUpdates();
	void KlockAttack();
};