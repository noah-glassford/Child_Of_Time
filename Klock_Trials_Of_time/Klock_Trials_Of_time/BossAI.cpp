#include "BossAI.h"

void BossObject::PickMovement()
{
	movementNumber =  rand() % 4; //Picks some number for movement
}

void BossObject::PickAttack()
{
	AttackNumber = rand() % 2;
	std::cout << AttackNumber;
}

void BossObject::RunAI()
{
	IncrementTimer();
	
	RunMovement();

	RunAttack();

	if (doMovement)
	{
		PickMovement();
		BossMovementTimer = 1.5;
	}
	if (doAttack)
	{
		PickAttack();
		BossAttackTimer = 3.f;
	}
}

void BossObject::IncrementTimer()
{
	if (BossMovementTimer > 0.f)
	{
		BossMovementTimer -= Timer::deltaTime;
		doMovement = false;
	}
	else if (BossMovementTimer <= 0.f)
	{
		BossMovementTimer = 0.f;
		doMovement = true;
		//std::cout << "bruh\n";	
	}

	if (BossAttackTimer > 0.f)
	{
		BossAttackTimer -= Timer::deltaTime;
		doAttack = 0;
	}
	else if (BossAttackTimer <= 0.f)
	{
		BossAttackTimer = 0.f;
		doAttack = 1;
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

void BossObject::RunAttack()
{
	switch (AttackNumber)
	{
	case 0:
		TestAttack();
	}
			
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
