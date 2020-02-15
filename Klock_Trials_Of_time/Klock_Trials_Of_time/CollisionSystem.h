#pragma once

#include "PhysicsSystem.h"
/*
This system is to make collision less shit
Also contains the collision listener
*/
class CollisionObject : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact);

	void EndContact(b2Contact* contact);
private:
	
};

inline void CollisionObject::BeginContact(b2Contact* contact)
{
	std::cout << "Bruh contact start\n";
}

inline void CollisionObject::EndContact(b2Contact* contact)
{
	std::cout << "Bruh contact end\n";
}