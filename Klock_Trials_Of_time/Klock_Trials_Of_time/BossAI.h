//Clark for the love of god don't touch these files

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


	void PickMovement(); //Picks which action the boss will do randomly

	void PickAttack();

	void RunAI(); //Function called every frame to run the AI

	void IncrementTimer(); //function that increments the timer, and changes member bool if action should happen
	
	void TestAttack();
	
	void MoveHorizontal(float velo);

	void MoveVertical(float velo);

	void RunAttack();

	void RunMovement();

	void AttackStraightProjectile(b2Vec2 velo);

	int AttackNumber;

	int movementNumber;

	int EntityNumber;

	float BossMovementTimer;

	float BossAttackTimer;

	bool doMovement;

	bool doAttack;

};