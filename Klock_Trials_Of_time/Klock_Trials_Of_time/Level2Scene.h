#pragma once
#include "Scene.h"
#include "SoundEngine.h"
class Level2Scene : public Scene
{
public:
	Level2Scene(std::string name);

	//Each Scene will need to have a different
	//init, as each scene's content will be different
	void InitScene(float windowWidth, float windowHeight) override;

	bool createdint{ 0 }; //Counts frames since created a combat hitbox
	unsigned int tempent;
	bool tempbool{ 1 };

	void Update();
};