#pragma once
#include "PlayerData.h"
#include "Timer.h"
#include "ECS.h"
#include <iostream>
#include <cstdlib>

class BossObject : public PlayerData
{
public:

	BossObject() {};


	void PickAction(); //Picks which action the boss will do randomly

	void RunAI(); //Function called every frame to run the AI

	void IncrementTimer(); //function that increments the timer, and changes member bool if action should happen
	
	void TestAttack();
	
	void MoveHorizontal(float velo);

	void MoveVertical(float velo);

	void RunMovement();

	int movementNumber;

	int EntityNumber;

	float BossActionTimer;

	bool doMovement;

};