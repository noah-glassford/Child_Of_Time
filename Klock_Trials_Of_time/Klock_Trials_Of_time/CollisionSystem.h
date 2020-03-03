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

	if ((int)fixtureBUserData == 8 && (int)fixtureAUserData == 7)
	{
		std::cout << ECS::GetComponent<PlayerData>(2).Health;
		ECS::GetComponent<PlayerData>(2).Health --;
	}

	if ((int)fixtureBUserData == 3) //Klock Footsensor	
		ECS::GetComponent<PlayerData>(1).Grounded = true;

	if ((int)fixtureBUserData == 4) //Klock right side of body sensor
		ECS::GetComponent<PlayerData>(1).OnWallRight = true;
	
	if ((int)fixtureBUserData == 5) //Klock left side of body sensor
		ECS::GetComponent<PlayerData>(1).OnWallLeft = true;

	if ((int)fixtureAUserData == 8 && (int)fixtureBUserData == 7)
	{
		std::cout << ECS::GetComponent<PlayerData>(2).Health;
		ECS::GetComponent<PlayerData>(2).Health --;
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

