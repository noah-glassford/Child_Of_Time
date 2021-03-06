#include "MovementSystem.h"

PhysicsBody MovementSystem::GetPhysicsBody()
{
	return PhysicsBod;
}

b2Body* MovementSystem::GetB2Body()
{
	return B2Body;
}

bool MovementSystem::GetIsTouching()
{
	return isTouching;
}

bool MovementSystem::GetIsAttacking()
{
	return PlData.isAttacking;
}


void MovementSystem::SetBothBodies(int entity)
{
	PhysicsBod = ECS::GetComponent<PhysicsBody>(entity);
	B2Body = PhysicsBod.GetBody();
}

void MovementSystem::SetPlayerData(int entity)
{
	PlData = ECS::GetComponent<PlayerData>(1);
}

void MovementSystem::SetPhysicsBody(PhysicsBody PhysBod)
{
	PhysicsBod = PhysBod;
}

void MovementSystem::SetB2Body(b2Body* body)
{
	B2Body = body;
}

void MovementSystem::SetIsTouching()
{
	if (PlData.Grounded == true)
		isTouching = true;
	else
		isTouching = false;
}

void MovementSystem::SetIsTouching(bool touching)
{
	isTouching = touching;
}

void MovementSystem::SetIsAttacking(bool Attacking)
{
	PlData.isAttacking = Attacking;
}




void MovementSystem::MoveLeft(float Force)
{	
	//PhysicsBod.ApplyForce(vec3(-Force * deltaTime, 0, 0));
	b2Vec2 velo = B2Body->GetLinearVelocity();
	velo.x = -Force;
	B2Body->SetLinearVelocity(velo);
}

void MovementSystem::MoveRight(float Force)
{
	//PhysicsBod.ApplyForce(vec3(Force * deltaTime, 0, 0));
	b2Vec2 velo = B2Body->GetLinearVelocity();
	velo.x = Force;
	B2Body->SetLinearVelocity(velo);
}

void MovementSystem::Jump(float Force)
{
	PhysicsBod.ApplyForce(vec3(0, Force, 0));
}

void MovementSystem::DownMove(float Force)
{
	PhysicsBod.ApplyForce(vec3(0, -Force, 0));
}

void MovementSystem::TeleportMovementLeft(float velocity)
{
	B2Body->SetTransform(b2Vec2(PhysicsBod.GetPosition().x - velocity, PhysicsBod.GetPosition().y), 0);
}

void MovementSystem::TeleportMovementRight(float velocity)
{
	B2Body->SetTransform(b2Vec2(PhysicsBod.GetPosition().x + velocity, PhysicsBod.GetPosition().y), 0);
}

void MovementSystem::TeleportMovementUp(float velocity)
{
	B2Body->SetTransform(b2Vec2(PhysicsBod.GetPosition().x, PhysicsBod.GetPosition().y + velocity), 0);
}

void MovementSystem::TeleportMovementDown(float velocity)
{
	B2Body->SetTransform(b2Vec2(PhysicsBod.GetPosition().x, PhysicsBod.GetPosition().y - velocity), 0);
}
