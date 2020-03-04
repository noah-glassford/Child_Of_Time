#pragma once

#include "PhysicsSystem.h"
#include "MovementSystem.h"
/*
This system is to make collision less shit
Also contains the collision listener
*/

class CollisionListener : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact);

	void EndContact(b2Contact* contact);
private:
};


inline void CollisionListener::BeginContact(b2Contact* contact)
{

	void* fixtureAUserData = contact->GetFixtureA()->GetUserData();
	void* fixtureBUserData = contact->GetFixtureB()->GetUserData();

	if ((int)fixtureAUserData == 3) //Klock Footsensor
	{
		ECS::GetComponent<PlayerData>(1).Grounded = true;
	}
	if ((int)fixtureAUserData == 4) //Klock right side of body sensor
		ECS::GetComponent<PlayerData>(1).OnWallRight = true;

	if ((int)fixtureAUserData == 5) //Klock left side of body sensor
		ECS::GetComponent<PlayerData>(1).OnWallLeft = true;

	if ((int)fixtureBUserData == 8 && (int)fixtureAUserData == 7) //Klock hit enemy
	{
		std::cout << ECS::GetComponent<PlayerData>(2).Health;
		ECS::GetComponent<PlayerData>(2).Health--;
	}

	if ((int)fixtureBUserData == 3) //Klock Footsensor	
		ECS::GetComponent<PlayerData>(1).Grounded = true;

	if ((int)fixtureBUserData == 4) //Klock right side of body sensor
		ECS::GetComponent<PlayerData>(1).OnWallRight = true;

	if ((int)fixtureBUserData == 5) //Klock left side of body sensor
		ECS::GetComponent<PlayerData>(1).OnWallLeft = true;

	if ((int)fixtureAUserData == 8 && (int)fixtureBUserData == 7) //klock hit enemy
	{
		std::cout << ECS::GetComponent<PlayerData>(2).Health;
		ECS::GetComponent<PlayerData>(2).Health--;
	}

	//all the klock getting hit by stuff
	if ((int)fixtureAUserData == 8 && (int)fixtureBUserData == 4)
	{
		std::cout << "Klock got hit by enemy on his right";
		ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
		ECS::GetComponent<PlayerData>(1).CanBeHit = 0;
		ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.f;
	}

	if ((int)fixtureBUserData == 8 && (int)fixtureAUserData == 4)
	{
		std::cout << "Klock got hit by enemy on his right";
		ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
		ECS::GetComponent<PlayerData>(1).CanBeHit = 0;
		ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.f;
	}

	if ((int)fixtureAUserData == 8 && (int)fixtureBUserData == 5)
	{
		std::cout << "Klock got hit by an enemy on his left";
		ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
		ECS::GetComponent<PlayerData>(1).CanBeHit = 0;
		ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.f;
	}
	if ((int)fixtureBUserData == 8 && (int)fixtureAUserData == 5)
	{
		std::cout << "Klock got hit by an enemy on his left";
		ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
		ECS::GetComponent<PlayerData>(1).CanBeHit = 0;
		ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.f;
	}
}
inline void CollisionListener::EndContact(b2Contact* contact)
{

	void* fixtureUserData = contact->GetFixtureA()->GetUserData();

	if ((int)fixtureUserData == 3) //Klock Footsensor
		ECS::GetComponent<PlayerData>(1).Grounded = false;

	if ((int)fixtureUserData == 4) //Klock right side of body sensor
		ECS::GetComponent< PlayerData>(1).OnWallRight = false;

	if ((int)fixtureUserData == 5) //Klock left side of body sensor

		ECS::GetComponent<PlayerData>(1).OnWallLeft = false;


	fixtureUserData = contact->GetFixtureB()->GetUserData();

	if ((int)fixtureUserData == 3) //Klock Footsensor
		ECS::GetComponent< PlayerData>(1).Grounded = false;

	if ((int)fixtureUserData == 4) //Klock right side of body sensor

		ECS::GetComponent<PlayerData>(1).OnWallRight = false;

	if ((int)fixtureUserData == 5) //Klock left side of body sensor

		ECS::GetComponent<PlayerData>(1).OnWallLeft = false;

}

