#include "BossAI.h"

void BossObject::PickMovement()
{
	movementNumber =  rand() % 4; //Picks some number for movement
}

void BossObject::PickAttack()
{
	AttackNumber = rand() % 3;
	SetAttackPosition(b2Vec2(-50, 0));
	std::cout << AttackNumber;
}

void BossObject::RunAI()
{
	
	//AttackStraightProjectile(b2Vec2(50, 0));

	IncrementTimer();
	
	RunMovement();

	RunAttack();

	if (doMovement)
	{
		PickMovement();
		BossMovementTimer = 1.0;
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
	//std::cout << "Test Attack";
}

void BossObject::MoveHorizontal(float velo)
{
	float velocity = velo;
	if (ECS::GetComponent<PlayerData>(1).isSlowed)
		velocity = velocity / 3;
	else
		velocity = velo;
	
	ECS::GetComponent<PhysicsBody>(EntityNumber).GetBody()->SetLinearVelocity(b2Vec2(velocity,0));
}

void BossObject::MoveVertical(float velo)
{
	float velocity = velo;
	if (ECS::GetComponent<PlayerData>(1).isSlowed)
		velocity = velocity / 3;
	else
		velocity = velo;
	
	ECS::GetComponent<PhysicsBody>(EntityNumber).GetBody()->SetLinearVelocity(b2Vec2(0,velocity));
}

void BossObject::RunAttack()
{
	
	switch (AttackNumber)
	{
	case 0:
		//TestAttack();
	case 1:

		float velocity = 50;
		if (ECS::GetComponent<PlayerData>(1).isSlowed)
			velocity = velocity / 3;
		else
			velocity = 50;
		ECS::GetComponent<PhysicsBody>(9).GetBody()->SetLinearVelocity(b2Vec2(-velocity,0));
		
	}
			
}

void BossObject::RunMovement()
{
	switch (movementNumber)
	{
	case 0:
		MoveHorizontal(50.f);
		break;
	case 1:
		MoveVertical(50.f);
		break;
	case 2:
		MoveVertical(-50.f);
		break;
	case 3:
		MoveHorizontal(-50.f);
		break;
	}
}

void BossObject::SetAttackPosition(b2Vec2 offset)
{
	ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(ECS::GetComponent<PhysicsBody>(2).GetPosition().x + offset.x,ECS::GetComponent<PhysicsBody>(2).GetPosition().y + offset.y),0);
}
