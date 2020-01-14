#pragma once
#include "Scene.h"
#include "Game.h"
class AssignemntScene : public Scene
{
public:
	AssignemntScene(std::string name);
	void InitScene(float windowWidth, float windowHeight) override;
};