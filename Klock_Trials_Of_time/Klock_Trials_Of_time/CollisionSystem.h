#pragma once
#include "Game.h"
/*
This system is to make collision less shit
Also contains the collision listener
*/
class CollisionObject
{
public:
	b2Body GetBody();
	void SetBody(b2Body* body);
	bool GetIsTouching();
	void SetIsTouching(bool touching);
private:
	b2Body* mainBody;
	bool isTouching;

};