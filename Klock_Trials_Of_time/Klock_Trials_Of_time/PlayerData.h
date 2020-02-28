#pragma once
/*
This class handles all the random bool spaghetti code shit and puts it all into once place
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
};