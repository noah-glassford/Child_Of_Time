#pragma once

#include "Scene.h"
#include "SoundEngine.h"
#include "Game.h"

class MainMenu : public Scene
{
public:
	MainMenu(std::string name);

	//Each Scene will need to have a different
	//init, as each scene's content will be different
	void InitScene(float windowWidth, float windowHeight) override;
	void Update() override;

};