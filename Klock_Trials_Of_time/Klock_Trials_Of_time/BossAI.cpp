#include "BossAI.h"

void BossObject::PickAction()
{
	
	int i =  rand() % 5; //Picks some action
	switch (i)
	{
	case 0: std::cout << "Case 0\n";
		break;
	case 1: std::cout << "Case 1\n";
		break;
	case 2: std::cout << "Case 2\n";
		break;
	case 3: std::cout << "Case 3\n";
		break;
	case 4: std::cout << "Case 4\n";
		break;
	}
	
}

void BossObject::RunAI()
{
	IncrementTimer();
	
	std::cout << BossActionTimer << std::endl;
	
	if (doAction)
	{
		PickAction();
		BossActionTimer = 3;
	}
}

void BossObject::IncrementTimer()
{
	
	if (BossActionTimer > 0.f)
	{
		BossActionTimer -= Timer::deltaTime;
		doAction = false;
	}
	else if (BossActionTimer <= 0.f)
	{
		BossActionTimer = 0.f;
		doAction = true;
		//std::cout << "bruh\n";
		
	}
}

void BossObject::TestAttack()
{
	std::cout << "Test Attack";
}
