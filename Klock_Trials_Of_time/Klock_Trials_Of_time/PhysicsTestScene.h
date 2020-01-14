#pragma once

#include "Scene.h"

class PhysicsTestScene : public Scene
{
public:
	PhysicsTestScene(std::string name);

	//Each Scene will need to have a different
	//init, as each scene's content will be different
	void InitScene(float windowWidth, float windowHeight) override;
};