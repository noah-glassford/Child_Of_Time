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
	bool createdint{ 0 };
	unsigned int tempent;
};