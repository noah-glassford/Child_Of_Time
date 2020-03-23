#include "BossAI.h"

void BossObject::PickAction()
{
	
	movementNumber =  rand() % 4; //Picks some action
	
}

void BossObject::RunAI()
{
	IncrementTimer();
	
	RunMovement();

	if (doMovement)
	{
		PickAction();
		BossActionTimer = 1.5;
	}
}

void BossObject::IncrementTimer()
{
	
	if (BossActionTimer > 0.f)
	{
		BossActionTimer -= Timer::deltaTime;
		doMovement = false;
	}
	else if (BossActionTimer <= 0.f)
	{
		BossActionTimer = 0.f;
		doMovement = true;
		//std::cout << "bruh\n";
		
	}
}

void BossObject::TestAttack()
{
	std::cout << "Test Attack";
}

void BossObject::MoveHorizontal(float velo)
{
	ECS::GetComponent<PhysicsBody>(EntityNumber).GetBody()->SetLinearVelocity(b2Vec2(velo,0));
}

void BossObject::MoveVertical(float velo)
{
	ECS::GetComponent<PhysicsBody>(EntityNumber).GetBody()->SetLinearVelocity(b2Vec2(0,velo));
}

void BossObject::RunMovement()
{
	switch (movementNumber)
	{
	case 0:
		MoveHorizontal(30.f);
		break;
	case 1:
		MoveVertical(30.f);
		break;
	case 2:
		MoveVertical(-30.f);
		break;
	case 3:
		MoveHorizontal(-30.f);
		break;
	}
}
