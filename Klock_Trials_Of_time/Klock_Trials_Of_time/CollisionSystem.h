#pragma once

#include "PhysicsSystem.h"
#include "MovementSystem.h"
/*
This system is to make collision less shit
Also contains the collision listener
*/

class CollisionObject
{

};

class CollisionListener : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact);

	void EndContact(b2Contact* contact);
private:
};

inline void CollisionListener::BeginContact(b2Contact* contact)
{

	void* fixtureUserData = contact->GetFixtureA()->GetUserData();

	if ((int)fixtureUserData == 3) //Klock Footsensor
	{
		ECS::GetComponent<PhysicsBody>(1).Grounded = true;
		
	}

	fixtureUserData = contact->GetFixtureB()->GetUserData();

	if ((int)fixtureUserData == 3) //Klock Footsensor
	{
		ECS::GetComponent<PhysicsBody>(1).Grounded = true;
		
	}
}

inline void CollisionListener::EndContact(b2Contact* contact)
{

	void* fixtureUserData = contact->GetFixtureA()->GetUserData();
	
	if ((int)fixtureUserData == 3) //Klock Footsensor
	{
		ECS::GetComponent<PhysicsBody>(1).Grounded = false;

	}


	fixtureUserData = contact->GetFixtureB()->GetUserData();
	if ((int)fixtureUserData == 3) //Klock Footsensor
	{
		ECS::GetComponent<PhysicsBody>(1).Grounded = false;
		std::cout << "bruh";
	}
}

