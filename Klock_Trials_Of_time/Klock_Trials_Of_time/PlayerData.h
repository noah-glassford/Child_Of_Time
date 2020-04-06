#pragma once
/*
This class handles all the random bool spaghetti code and puts it all into once place
This is an ECS component that will be attached to stuff
This is unorganized and not done properly but that doesn't really matter its just a bunch of bools
*/

class PlayerData
{
public:
	//Could I make this private? (The proper way) Yes. Am I going to? No.
	bool Grounded;

	bool OnWallRight;

	bool OnWallLeft;

	bool isAttacking{ 0 };

	bool canjump;//This is literally only used to make playing with a controller not feel bad

	int Health{ 3 };

	float TimeSinceAtt{ 0 };

	bool CanAttack = 1;

	int CurrentScene;
	bool facingLeft{ 0 };

	bool isSlowed;

	float UsedUpTime{ 0 };

	float TimeSinceHit = 0.f;

	bool Hit{ 0 };

	bool canUseTimeSlow = false;
};