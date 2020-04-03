//da supa secret boss fight that is finna b retextured the day before submission :)
#pragma once
#pragma once

#include "Scene.h"
#include "SoundEngine.h"
#include "BossAI.h"

class BossFightScene : public Scene
{
public:
	BossFightScene(std::string name);

	//Each Scene will need to have a different
	//init, as each scene's content will be different
	void InitScene(float windowWidth, float windowHeight) override;
	void Update() override;
	bool createdint{ 0 };
	unsigned int tempent;
	float windowWidthtemp;
	float windowHeighttemp;
};