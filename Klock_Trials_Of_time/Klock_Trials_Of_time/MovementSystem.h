#pragma once
#include "PhysicsBody.h"
#include "ECS.h"
#include "Timer.h"
/*
This class is likely useless, if it is I'll just get rid of it later, but I want to try to see if I can clean everything up a bit
Im sorta just creating one class that handles all the data required for movement to simplify moving stuff
*/
class MovementSystem : public Timer //Create an object of this class in game.cpp for every object that will be moving, EX: MovementSystem Klock; for klock movement
{
public:
	//Getters and Setters
	PhysicsBody GetPhysicsBody();
	b2Body* GetB2Body();
	bool GetIsTouching();
	
	void SetBothBodies(int entity);//Sets the ECS physicsbody, and then also automatically sets the b2body to be the same
	void SetPhysicsBody(PhysicsBody PhysBod);
	void SetB2Body(b2Body* body);
	void SetIsTouching();//Specifically made for klock foot sensor
	void SetIsTouching(bool touching);//Overloaded function for other things

	//Member Function aka actual movement
	void MoveLeft(float Force);//Uses apply force to move the object left
	void MoveRight(float Force);//Uses apply force to move the object right
	void Jump(float Force);//Uses apply force to jump
	void DownMove(float Force);//Currently used for a ground pound for Klock
	void TeleportMovementLeft(float velocity);//Mostly Used for moving platforms
	void TeleportMovementRight(float velocity);//Mostly used for moving platforms
	void TeleportMovementUp(float velocity);
	void TeleportMovementDown(float velocity);

private:
	PhysicsBody PhysicsBod; //The framework physicsbody
	b2Body* B2Body; //Box2D body
	bool isTouching;

};