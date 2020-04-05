#pragma once
/*
22 23 for level 1
*/
#include "PhysicsSystem.h"
#include "MovementSystem.h"
#include "SoundEngine.h"
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

	Sound2D _EnemyHurt("enemyhurt.wav", "group1");

	if ((int)fixtureAUserData == 3) //Klock Footsensor
	{
		ECS::GetComponent<PlayerData>(1).Grounded = true;
		ECS::GetComponent<AnimationController>(1).SetActiveAnim(0);
	}
	if ((int)fixtureAUserData == 4) //Klock right side of body sensor
		ECS::GetComponent<PlayerData>(1).OnWallRight = true;

	if ((int)fixtureAUserData == 5) //Klock left side of body sensor
		ECS::GetComponent<PlayerData>(1).OnWallLeft = true;

	if ((int)fixtureBUserData == 8 && (int)fixtureAUserData == 7) //Klock hit enemy
	{
		if (ECS::GetComponent<PlayerData>(1).CurrentScene == 3)
		{
			ECS::GetComponent<PlayerData>(2).Health--;
			_EnemyHurt.play();
			//std::cout << "fuck";
		}
		std::cout << ECS::GetComponent<PlayerData>(2).Health;
	}

	if ((int)fixtureBUserData == 3) //Klock Footsensor	
		ECS::GetComponent<PlayerData>(1).Grounded = true;

	if ((int)fixtureBUserData == 4) //Klock right side of body sensor
		ECS::GetComponent<PlayerData>(1).OnWallRight = true;

	if ((int)fixtureBUserData == 5) //Klock left side of body sensor
		ECS::GetComponent<PlayerData>(1).OnWallLeft = true;

	if ((int)fixtureAUserData == 8 && (int)fixtureBUserData == 7) //klock hit enemy
	{
		if (ECS::GetComponent<PlayerData>(1).CurrentScene == 3)
		{
			ECS::GetComponent<PlayerData>(2).Health--;
			_EnemyHurt.play();
			//std::cout << "fuck";
		}
		std::cout << ECS::GetComponent<PlayerData>(2).Health;
	}

	//all the klock getting hit by stuff
	if ((int)fixtureAUserData == 8 && (int)fixtureBUserData == 4)
	{
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(-20000000, 1000000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(-30, 30));
			//if (ECS::GetComponent<PlayerData>(1).CurrentScene == 3)
				//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
		}
	
		
			
		
		
	}

	if ((int)fixtureBUserData == 8 && (int)fixtureAUserData == 4)
	{
		//std::cout << "Klock got hit by enemy on his right";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(-20000000, 100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(-30, 30));
			//if (ECS::GetComponent<PlayerData>(1).CurrentScene == 3)
				//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
		}

	}
	if ((int)fixtureAUserData == 8 && (int)fixtureBUserData == 5)
	{
		//std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(20000000, 100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//if (ECS::GetComponent<PlayerData>(1).CurrentScene == 3)
				//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999,-9999),0);
		}	
	}


	if ((int)fixtureBUserData == 8 && (int)fixtureAUserData == 5)
	{
	//	std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(200000000,100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//if (ECS::GetComponent<PlayerData>(1).CurrentScene == 3)
				//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
		}
	
	}
	
	
	//all the klock getting hit by stuff(for straight line projectile
	if ((int)fixtureAUserData == 10 && (int)fixtureBUserData == 4)
	{
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(-20000000, 1000000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(-30, 30));
			//if (ECS::GetComponent<PlayerData>(1).CurrentScene == 3)
				//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			//ECS::GetComponent<PhysicsBody>(10).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}

	//	

	

	}

	if ((int)fixtureBUserData == 10 && (int)fixtureAUserData == 4)
	{
		//std::cout << "Klock got hit by enemy on his right";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(-20000000, 100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(-30, 30));
			//if (ECS::GetComponent<PlayerData>(1).CurrentScene == 3)
				//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			//ECS::GetComponent<PhysicsBody>(10).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		
		//
	}
	if ((int)fixtureAUserData == 10 && (int)fixtureBUserData == 5)
	{
		//std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(20000000, 100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(9).SetPosition(b2Vec2(-999, 999));
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
		

	}
	if ((int)fixtureBUserData == 10 && (int)fixtureAUserData == 5)
	{
		//	std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(200000000,100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(10).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
	//	ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
	
	}

	if ((int)fixtureBUserData == 11 && (int)fixtureAUserData == 7)
	{
		_EnemyHurt.play();
		
		ECS::GetComponent<PlayerData>(23).Health--;
		if (!ECS::GetComponent<PlayerData>(1).facingLeft)
			ECS::GetComponent<PhysicsBody>(23).GetBody()->SetLinearVelocity(b2Vec2(20, 20));
		else
			ECS::GetComponent<PhysicsBody>(23).GetBody()->SetLinearVelocity(b2Vec2(-20, 20));
	}
	if ((int)fixtureAUserData == 11 && (int)fixtureBUserData == 7)
	{
		ECS::GetComponent<PlayerData>(23).Health--;
		if (!ECS::GetComponent<PlayerData>(1).facingLeft)
			ECS::GetComponent<PhysicsBody>(23).GetBody()->SetLinearVelocity(b2Vec2(20, 20));
		else
			ECS::GetComponent<PhysicsBody>(23).GetBody()->SetLinearVelocity(b2Vec2(-20, 20));
		
		std::cout << "bruh";
	}
	if ((int)fixtureBUserData == 12 && (int)fixtureAUserData == 7)
	{
		ECS::GetComponent<PlayerData>(24).Health--;
		if (!ECS::GetComponent<PlayerData>(1).facingLeft)
			ECS::GetComponent<PhysicsBody>(24).GetBody()->SetLinearVelocity(b2Vec2(20, 20));
		else
			ECS::GetComponent<PhysicsBody>(24).GetBody()->SetLinearVelocity(b2Vec2(-20, 20));
		std::cout << "bruh";
	}
	if ((int)fixtureAUserData == 12 && (int)fixtureBUserData == 7)
	{
		ECS::GetComponent<PlayerData>(24).Health--;
		if (!ECS::GetComponent<PlayerData>(1).facingLeft)
			ECS::GetComponent<PhysicsBody>(24).GetBody()->SetLinearVelocity(b2Vec2(20, 20));
		else
			ECS::GetComponent<PhysicsBody>(24).GetBody()->SetLinearVelocity(b2Vec2(-20, 20));
	}

	//all the klock getting hit by stuff(for straight line projectile
	if ((int)fixtureAUserData == 11 && (int)fixtureBUserData == 4)
	{
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(-20000000, 1000000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(-30, 30));
			//if (ECS::GetComponent<PlayerData>(1).CurrentScene == 3)
				//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			//ECS::GetComponent<PhysicsBody>(10).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}

		//	



	}

	if ((int)fixtureBUserData == 11 && (int)fixtureAUserData == 4)
	{
		//std::cout << "Klock got hit by enemy on his right";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(-20000000, 100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(-30, 30));
			//if (ECS::GetComponent<PlayerData>(1).CurrentScene == 3)
				//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			//ECS::GetComponent<PhysicsBody>(10).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}

		//
	}
	//all the klock getting hit by stuff(for straight line projectile
	if ((int)fixtureAUserData == 11 && (int)fixtureBUserData == 5)
	{
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(-20000000, 1000000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(-30, 30));
			//if (ECS::GetComponent<PlayerData>(1).CurrentScene == 3)
				//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			//ECS::GetComponent<PhysicsBody>(10).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}

		//	



	}

	if ((int)fixtureBUserData == 11 && (int)fixtureAUserData == 5)
	{
		//std::cout << "Klock got hit by enemy on his right";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(-20000000, 100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(-30, 30));
			//if (ECS::GetComponent<PlayerData>(1).CurrentScene == 3)
				//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			//ECS::GetComponent<PhysicsBody>(10).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}

		//
	}
	if ((int)fixtureAUserData == 12 && (int)fixtureBUserData == 5)
	{
		//std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(20000000, 100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(9).SetPosition(b2Vec2(-999, 999));
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);


	}
	if ((int)fixtureBUserData == 12 && (int)fixtureAUserData == 5)
	{
		//	std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(200000000,100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(10).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//	ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);

	}

	if ((int)fixtureAUserData == 12 && (int)fixtureBUserData == 4)
	{
		//std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(20000000, 100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(9).SetPosition(b2Vec2(-999, 999));
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);


	}
	if ((int)fixtureBUserData == 12 && (int)fixtureAUserData == 4)
	{
		//	std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(200000000,100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(10).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//	ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);

	}

	if ((int)fixtureAUserData == 13 && (int)fixtureBUserData == 5)
	{
		//std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(20000000, 100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(9).SetPosition(b2Vec2(-999, 999));
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);


	}
	if ((int)fixtureBUserData == 13 && (int)fixtureAUserData == 5)
	{
		//	std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(200000000,100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(10).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//	ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);

	}

	if ((int)fixtureAUserData == 13 && (int)fixtureBUserData == 4)
	{
		//std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(20000000, 100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(9).SetPosition(b2Vec2(-999, 999));
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);


	}
	if ((int)fixtureBUserData == 13 && (int)fixtureAUserData == 4)
	{
		//	std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(200000000,100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(10).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//	ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);

	}

	if ((int)fixtureAUserData == 14 && (int)fixtureBUserData == 5)
	{
		//std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(20000000, 100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(9).SetPosition(b2Vec2(-999, 999));
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);


	}
	if ((int)fixtureBUserData == 14 && (int)fixtureAUserData == 5)
	{
		//	std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(200000000,100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(10).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//	ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);

	}

	if ((int)fixtureAUserData == 14 && (int)fixtureBUserData == 4)
	{
		//std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(20000000, 100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(9).SetPosition(b2Vec2(-999, 999));
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);


	}
	if ((int)fixtureBUserData == 14 && (int)fixtureAUserData == 4)
	{
		//	std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(200000000,100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(10).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//	ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);

	}

	if ((int)fixtureAUserData == 15 && (int)fixtureBUserData == 5)
	{
		//std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(20000000, 100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(9).SetPosition(b2Vec2(-999, 999));
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);


	}
	if ((int)fixtureBUserData == 15 && (int)fixtureAUserData == 5)
	{
		//	std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(200000000,100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(10).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//	ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);

	}

	if ((int)fixtureAUserData == 15 && (int)fixtureBUserData == 4)
	{
		//std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(20000000, 100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(9).SetPosition(b2Vec2(-999, 999));
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);


	}
	if ((int)fixtureBUserData == 15 && (int)fixtureAUserData == 4)
	{
		//	std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(200000000,100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(10).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//	ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);

	}

	if ((int)fixtureAUserData == 16 && (int)fixtureBUserData == 5)
	{
		//std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(20000000, 100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(9).SetPosition(b2Vec2(-999, 999));
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);


	}
	if ((int)fixtureBUserData == 16 && (int)fixtureAUserData == 5)
	{
		//	std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(200000000,100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(10).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//	ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);

	}

	if ((int)fixtureAUserData == 16 && (int)fixtureBUserData == 4)
	{
		//std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(20000000, 100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(9).SetPosition(b2Vec2(-999, 999));
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);


	}
	if ((int)fixtureBUserData == 16 && (int)fixtureAUserData == 4)
	{
		//	std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(200000000,100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(10).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//	ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);

	}

	if ((int)fixtureAUserData == 17 && (int)fixtureBUserData == 5)
	{
		//std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(20000000, 100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(9).SetPosition(b2Vec2(-999, 999));
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);


	}
	if ((int)fixtureBUserData == 17 && (int)fixtureAUserData == 5)
	{
		//	std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(200000000,100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(10).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//	ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);

	}

	if ((int)fixtureAUserData == 17 && (int)fixtureBUserData == 4)
	{
		//std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(20000000, 100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(9).SetPosition(b2Vec2(-999, 999));
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);


	}
	if ((int)fixtureBUserData == 17 && (int)fixtureAUserData == 4)
	{
		//	std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(200000000,100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(10).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//	ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);

	}

	if ((int)fixtureBUserData == 13 && (int)fixtureAUserData == 7)
	{
		_EnemyHurt.play();

		ECS::GetComponent<PlayerData>(36).Health--;
		if (!ECS::GetComponent<PlayerData>(1).facingLeft)
			ECS::GetComponent<PhysicsBody>(36).GetBody()->SetLinearVelocity(b2Vec2(20, 20));
		else
			ECS::GetComponent<PhysicsBody>(36).GetBody()->SetLinearVelocity(b2Vec2(-20, 20));
	}

	if ((int)fixtureAUserData == 13 && (int)fixtureBUserData == 7)
	{
		_EnemyHurt.play();

		ECS::GetComponent<PlayerData>(36).Health--;
		if (!ECS::GetComponent<PlayerData>(1).facingLeft)
			ECS::GetComponent<PhysicsBody>(36).GetBody()->SetLinearVelocity(b2Vec2(20, 20));
		else
			ECS::GetComponent<PhysicsBody>(36).GetBody()->SetLinearVelocity(b2Vec2(-20, 20));
	}


	if ((int)fixtureBUserData == 14 && (int)fixtureAUserData == 7)
	{
		_EnemyHurt.play();

		ECS::GetComponent<PlayerData>(37).Health--;
		if (!ECS::GetComponent<PlayerData>(1).facingLeft)
			ECS::GetComponent<PhysicsBody>(37).GetBody()->SetLinearVelocity(b2Vec2(20, 20));
		else
			ECS::GetComponent<PhysicsBody>(37).GetBody()->SetLinearVelocity(b2Vec2(-20, 20));
	}

	if ((int)fixtureAUserData == 14 && (int)fixtureBUserData == 7)
	{
		_EnemyHurt.play();

		ECS::GetComponent<PlayerData>(37).Health--;
		if (!ECS::GetComponent<PlayerData>(1).facingLeft)
			ECS::GetComponent<PhysicsBody>(37).GetBody()->SetLinearVelocity(b2Vec2(20, 20));
		else
			ECS::GetComponent<PhysicsBody>(37).GetBody()->SetLinearVelocity(b2Vec2(-20, 20));
	}


	if ((int)fixtureBUserData == 15 && (int)fixtureAUserData == 7)
	{
		_EnemyHurt.play();

		ECS::GetComponent<PlayerData>(38).Health--;
		if (!ECS::GetComponent<PlayerData>(1).facingLeft)
			ECS::GetComponent<PhysicsBody>(38).GetBody()->SetLinearVelocity(b2Vec2(20, 20));
		else
			ECS::GetComponent<PhysicsBody>(38).GetBody()->SetLinearVelocity(b2Vec2(-20, 20));
	}

	if ((int)fixtureAUserData == 15 && (int)fixtureBUserData == 7)
	{
		_EnemyHurt.play();

		ECS::GetComponent<PlayerData>(38).Health--;
		if (!ECS::GetComponent<PlayerData>(1).facingLeft)
			ECS::GetComponent<PhysicsBody>(38).GetBody()->SetLinearVelocity(b2Vec2(20, 20));
		else
			ECS::GetComponent<PhysicsBody>(38).GetBody()->SetLinearVelocity(b2Vec2(-20, 20));
	}


	if ((int)fixtureBUserData == 16 && (int)fixtureAUserData == 7)
	{
		_EnemyHurt.play();

		ECS::GetComponent<PlayerData>(39).Health--;
		if (!ECS::GetComponent<PlayerData>(1).facingLeft)
			ECS::GetComponent<PhysicsBody>(39).GetBody()->SetLinearVelocity(b2Vec2(20, 20));
		else
			ECS::GetComponent<PhysicsBody>(39).GetBody()->SetLinearVelocity(b2Vec2(-20, 20));
	}

	if ((int)fixtureAUserData == 16 && (int)fixtureBUserData == 7)
	{
		_EnemyHurt.play();

		ECS::GetComponent<PlayerData>(39).Health--;
		if (!ECS::GetComponent<PlayerData>(1).facingLeft)
			ECS::GetComponent<PhysicsBody>(39).GetBody()->SetLinearVelocity(b2Vec2(20, 20));
		else
			ECS::GetComponent<PhysicsBody>(39).GetBody()->SetLinearVelocity(b2Vec2(-20, 20));
	}


	if ((int)fixtureBUserData == 17 && (int)fixtureAUserData == 7)
	{
		_EnemyHurt.play();

		ECS::GetComponent<PlayerData>(40).Health--;
		if (!ECS::GetComponent<PlayerData>(1).facingLeft)
			ECS::GetComponent<PhysicsBody>(40).GetBody()->SetLinearVelocity(b2Vec2(20, 20));
		else
			ECS::GetComponent<PhysicsBody>(40).GetBody()->SetLinearVelocity(b2Vec2(-20, 20));
	}

	if ((int)fixtureAUserData == 17 && (int)fixtureBUserData == 7)
	{
		_EnemyHurt.play();

		ECS::GetComponent<PlayerData>(40).Health--;
		if (!ECS::GetComponent<PlayerData>(1).facingLeft)
			ECS::GetComponent<PhysicsBody>(40).GetBody()->SetLinearVelocity(b2Vec2(20, 20));
		else
			ECS::GetComponent<PhysicsBody>(40).GetBody()->SetLinearVelocity(b2Vec2(-20, 20));
	}

	if ((int)fixtureBUserData == 18 && (int)fixtureAUserData == 7)
	{
		_EnemyHurt.play();

		ECS::GetComponent<PlayerData>(50).Health--;
		if (!ECS::GetComponent<PlayerData>(1).facingLeft)
			ECS::GetComponent<PhysicsBody>(50).GetBody()->SetLinearVelocity(b2Vec2(20, 20));
		else
			ECS::GetComponent<PhysicsBody>(50).GetBody()->SetLinearVelocity(b2Vec2(-20, 20));
	}

	if ((int)fixtureAUserData == 18 && (int)fixtureBUserData == 7)
	{
		_EnemyHurt.play();

		ECS::GetComponent<PlayerData>(50).Health--;
		if (!ECS::GetComponent<PlayerData>(1).facingLeft)
			ECS::GetComponent<PhysicsBody>(50).GetBody()->SetLinearVelocity(b2Vec2(20, 20));
		else
			ECS::GetComponent<PhysicsBody>(50).GetBody()->SetLinearVelocity(b2Vec2(-20, 20));
	}


	if ((int)fixtureBUserData == 19 && (int)fixtureAUserData == 7)
	{
		_EnemyHurt.play();

		ECS::GetComponent<PlayerData>(51).Health--;
		if (!ECS::GetComponent<PlayerData>(1).facingLeft)
			ECS::GetComponent<PhysicsBody>(51).GetBody()->SetLinearVelocity(b2Vec2(20, 20));
		else
			ECS::GetComponent<PhysicsBody>(51).GetBody()->SetLinearVelocity(b2Vec2(-20, 20));
	}

	if ((int)fixtureAUserData == 19 && (int)fixtureBUserData == 7)
	{
		_EnemyHurt.play();

		ECS::GetComponent<PlayerData>(51).Health--;
		if (!ECS::GetComponent<PlayerData>(1).facingLeft)
			ECS::GetComponent<PhysicsBody>(51).GetBody()->SetLinearVelocity(b2Vec2(20, 20));
		else
			ECS::GetComponent<PhysicsBody>(51).GetBody()->SetLinearVelocity(b2Vec2(-20, 20));
	}


	if ((int)fixtureBUserData == 20 && (int)fixtureAUserData == 7)
	{
		_EnemyHurt.play();

		ECS::GetComponent<PlayerData>(52).Health--;
		if (!ECS::GetComponent<PlayerData>(1).facingLeft)
			ECS::GetComponent<PhysicsBody>(52).GetBody()->SetLinearVelocity(b2Vec2(20, 20));
		else
			ECS::GetComponent<PhysicsBody>(52).GetBody()->SetLinearVelocity(b2Vec2(-20, 20));
	}

	if ((int)fixtureAUserData == 20 && (int)fixtureBUserData == 7)
	{
		_EnemyHurt.play();

		ECS::GetComponent<PlayerData>(52).Health--;
		if (!ECS::GetComponent<PlayerData>(1).facingLeft)
			ECS::GetComponent<PhysicsBody>(52).GetBody()->SetLinearVelocity(b2Vec2(20, 20));
		else
			ECS::GetComponent<PhysicsBody>(52).GetBody()->SetLinearVelocity(b2Vec2(-20, 20));
	}


	if ((int)fixtureAUserData == 18 && (int)fixtureBUserData == 4)
	{
		//std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(20000000, 100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(9).SetPosition(b2Vec2(-999, 999));
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);


	}
	if ((int)fixtureBUserData == 18 && (int)fixtureAUserData == 4)
	{
		//	std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(200000000,100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(10).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//	ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);

	}

	if ((int)fixtureAUserData == 18 && (int)fixtureBUserData == 5)
	{
		//std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(20000000, 100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(9).SetPosition(b2Vec2(-999, 999));
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);


	}
	if ((int)fixtureBUserData == 18 && (int)fixtureAUserData == 5)
	{
		//	std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(200000000,100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(10).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//	ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);

	}


	if ((int)fixtureAUserData == 19 && (int)fixtureBUserData == 4)
	{
		//std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(20000000, 100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(9).SetPosition(b2Vec2(-999, 999));
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);


	}
	if ((int)fixtureBUserData == 19 && (int)fixtureAUserData == 4)
	{
		//	std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(200000000,100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(10).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//	ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);

	}

	if ((int)fixtureAUserData == 19 && (int)fixtureBUserData == 5)
	{
		//std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(20000000, 100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(9).SetPosition(b2Vec2(-999, 999));
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);


	}
	if ((int)fixtureBUserData == 19 && (int)fixtureAUserData == 5)
	{
		//	std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(200000000,100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(10).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//	ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);

	}


	if ((int)fixtureAUserData == 20 && (int)fixtureBUserData == 4)
	{
		//std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(20000000, 100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(9).SetPosition(b2Vec2(-999, 999));
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);


	}
	if ((int)fixtureBUserData == 20 && (int)fixtureAUserData == 4)
	{
		//	std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(200000000,100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(10).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//	ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);

	}

	if ((int)fixtureAUserData == 20 && (int)fixtureBUserData == 5)
	{
		//std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(20000000, 100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(9).SetPosition(b2Vec2(-999, 999));
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);


	}
	if ((int)fixtureBUserData == 20 && (int)fixtureAUserData == 5)
	{
		//	std::cout << "Klock got hit by an enemy on his left";
		if (ECS::GetComponent<PlayerData>(1).Hit)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Health--;
			ECS::GetComponent<PlayerData>(1).Hit = 0;
			ECS::GetComponent<PlayerData>(1).TimeSinceHit = 0.7f;
			//ECS::GetComponent<PhysicsBody>(1).ApplyForce(vec3(200000000,100000000, 0));
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(30, 30));
			//ECS::GetComponent<PhysicsBody>(10).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);
			ECS::GetComponent<PhysicsBody>(10).moveonnextstep = true;
		}
		//	ECS::GetComponent<PhysicsBody>(9).GetBody()->SetTransform(b2Vec2(-9999, -9999), 0);

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
	{
		ECS::GetComponent< PlayerData>(1).Grounded = false;
		ECS::GetComponent<AnimationController>(1).GetAnimation(1).Reset();
		ECS::GetComponent<AnimationController>(1).GetAnimation(5).Reset();
	}

	if ((int)fixtureUserData == 4) //Klock right side of body sensor

		ECS::GetComponent<PlayerData>(1).OnWallRight = false;

	if ((int)fixtureUserData == 5) //Klock left side of body sensor

		ECS::GetComponent<PlayerData>(1).OnWallLeft = false;


	void* fixtureAUserData = contact->GetFixtureA()->GetUserData();
	void* fixtureBUserData = contact->GetFixtureB()->GetUserData();
	
	//all the klock getting hit by stuff
	if ((int)fixtureAUserData == 8 && (int)fixtureBUserData == 4)
	{

	}

	if ((int)fixtureBUserData == 8 && (int)fixtureAUserData == 4)
	{
	
	}

	if ((int)fixtureAUserData == 8 && (int)fixtureBUserData == 5)
	{
	
	}
	if ((int)fixtureBUserData == 8 && (int)fixtureAUserData == 5)
	{
		
	}

}

