#include "Level2Scene.h"

float platAcc;

float plat4MoveSpeed = 12;
float plat6MoveSpeed = 0;
float plat1819MoveSpeed = -100;
float plat23MoveSpeed = 24;
float plat27MoveSpeed = 20;
float plat34MoveSpeed = -80;
float plat42MoveSpeed = -80;
float plat45MoveSpeed = -120;

float enemy1Distance;
float enemy2Distance;
float enemy3Distance;
float enemy4Distance;

Level2Scene::Level2Scene(std::string name)
{
	m_gravity = b2Vec2(float32(0.f), float32(-60.f));
	m_physicsWorld->SetGravity(m_gravity);
	m_physicsWorld->SetContactListener(&listener);
}

void Level2Scene::InitScene(float windowWidth, float windowHeight)
{
	//Allocates Register
	m_sceneReg = new entt::registry;

	//Attach the Register
	ECS::AttachRegister(m_sceneReg);
	/*
	if (m_physicsWorld == nullptr)
	{
		m_physicsWorld = new b2World(m_gravity);
		m_physicsWorld->SetGravity(m_gravity);
		m_physicsWorld->SetContactListener(&listener);
	}
	*/

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	//Background ent 0
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "LV2BACK.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 6400, 700);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 200.f, 1.f));

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "BackGround");
	}

	//Setup klock, entity 1
		//Klock entity, used by the player, core entity #1
	{
		//Create new Entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PlayerData>(entity);

		//Sets up components
		std::string fileName = "KlockSpriteSheet.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);

		//Walking left animation
		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		animController.SetActiveAnim(0);
		auto& idle = animController.GetAnimation(0);
		//Idle Animation
		for (int i = 0; i <= 15; i++)
		{
			idle.AddFrame(vec2(806 * i, 914), vec2(806 * i + 806, 0));
		}
		idle.SetRepeating(true);
		idle.SetSecPerFrame(0.05f);

		//jump right animation
		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		animController.SetActiveAnim(1);
		auto& jump = animController.GetAnimation(1);
		for (int i = 16; i <= 20; i++)
		{
			jump.AddFrame(vec2(806 * i, 914), vec2(806 * i + 806, 0));
		}
		jump.SetRepeating(false);
		jump.SetSecPerFrame(0.05f);

		//walk right animation
		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		animController.SetActiveAnim(2);
		auto& walkRight = animController.GetAnimation(2);

		for (int i = 21; i <= 30; i++)
		{
			walkRight.AddFrame(vec2(806 * i, 914), vec2(806 * i + 806, 0));
		}
		walkRight.SetRepeating(true);
		walkRight.SetSecPerFrame(0.05f);

		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		animController.SetActiveAnim(3);
		auto& attackRight = animController.GetAnimation(3);
		for (int i = 31; i <= 38; i++)
		{
			attackRight.AddFrame(vec2(806 * i, 914), vec2(806 * i + 806, 0));
		}
		attackRight.SetRepeating(false);
		attackRight.SetSecPerFrame(0.05f);

		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		animController.SetActiveAnim(4);

		auto& IdleLeft = animController.GetAnimation(4);
		for (int i = 0; i <= 15; i++)
		{
			IdleLeft.AddFrame(vec2(806 * i + 806, 914), vec2(806 * i, 0));
		}
		IdleLeft.SetRepeating(true);
		IdleLeft.SetSecPerFrame(0.05f);

		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		animController.SetActiveAnim(5);

		auto& jumpLeft = animController.GetAnimation(5);
		for (int i = 16; i <= 20; i++)
		{
			jumpLeft.AddFrame(vec2(806 * i + 806, 914), vec2(806 * i, 0));
		}
		jumpLeft.SetRepeating(false);
		jumpLeft.SetSecPerFrame(0.05f);

		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		animController.SetActiveAnim(6);

		auto& runLeft = animController.GetAnimation(6);
		for (int i = 21; i <= 30; i++)
		{
			runLeft.AddFrame(vec2(806 * i + 806, 914), vec2(806 * i, 0));
		}
		runLeft.SetRepeating(true);
		runLeft.SetSecPerFrame(0.05f);

		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		animController.SetActiveAnim(7);

		auto& punchLeft = animController.GetAnimation(7);
		for (int i = 31; i <= 38; i++)
		{
			punchLeft.AddFrame(vec2(806 * i + 806, 914), vec2(806 * i, 0));
		}
		punchLeft.SetRepeating(false);
		punchLeft.SetSecPerFrame(0.05f);

		//Sets up components
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 35, 45, true, &animController);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(550.f, 0.f, 49.f));

		//Grabs reference to various components
		//Sets up components
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 49.f));
		ECS::GetComponent<PlayerData>(entity).Health = 6;
		//Grabs reference to various components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetHeight() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0.f), float32(0.f));
		tempDef.fixedRotation = true;

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), true, 1.5f);

		//fixture definition
		b2PolygonShape polygonShape;
		b2FixtureDef myFixtureDef;
		myFixtureDef.shape = &polygonShape;
		myFixtureDef.density = 3;
		//myFixtureDef.friction = 1.f;

		//Adds a foot sensor fixture under the body
		polygonShape.SetAsBox(12.f, 0.0001, b2Vec2(0.f, -25.f), 0);
		myFixtureDef.isSensor = true;
		b2Fixture* footSensorFixture = tempPhysBody.GetBody()->CreateFixture(&myFixtureDef);
		footSensorFixture->SetUserData((void*)3);

		//Adds a fixture the right side of the body
		polygonShape.SetAsBox(0.001f, 22.f, b2Vec2(17.6f, 0.f), 0);
		myFixtureDef.isSensor = true;
		b2Fixture* RightSideFixture = tempPhysBody.GetBody()->CreateFixture(&myFixtureDef);
		RightSideFixture->SetUserData((void*)4);

		//Adds a fixture the left side of the body
		polygonShape.SetAsBox(0.001, 22.f, b2Vec2(-17.6f, 0.f), 0);
		myFixtureDef.isSensor = true;
		b2Fixture* LeftSideFixture = tempPhysBody.GetBody()->CreateFixture(&myFixtureDef);
		LeftSideFixture->SetUserData((void*)5);

		//Sets up the identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Klock");
	}

	ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).canUseTimeSlow = true; // make sure this is called after the player is built

	{//Creates camera entity, entity 2
		auto entity = ECS::CreateEntity();
		EntityIdentifier::MainCamera(entity);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);
		ECS::AttachComponent<Transform>(entity);

		vec4 temp = ECS::GetComponent<Camera>(entity).GetOrthoSize();
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);
		ECS::GetComponent<Camera>(entity).Zoom(-140);

		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<HorizontalScroll>(entity).SetOffset(15.f);
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetOffset(15.f);

		//Sets up identifier
		unsigned int bitHolder = EntityIdentifier::HoriScrollCameraBit() | EntityIdentifier::VertScrollCameraBit() | EntityIdentifier::CameraBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Main Scrolling Camera");
		ECS::SetIsMainCamera(entity, true);
	}

	//setup platform that you start on, entity 3
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 50);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(0.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}

	//setup platform for testing moving platforms, entity 4
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 20);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_kinematicBody;
		tempDef.position.Set(float32(125.f), float32(100.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 24), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}

	//setup platform used after the first moving platform, entity 5
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 50);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(250.f), float32(150.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform to use as a door, entity 6
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 160);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_kinematicBody;
		tempDef.position.Set(float32(350.f), float32(400.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 24), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup platform used after the first door, entity 7
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 50);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(450.f), float32(150.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform #2 used after the first door, entity 8
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 50);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(600.f), float32(225.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform #3 used after the first door, entity 9
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 50);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(450.f), float32(300.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform used as you return from the door, entity 10
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 50);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(200.f), float32(325.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform #2 used as you return from the door, entity 11
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 50);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(40.f), float32(410.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform #3 used as you return from the door, entity 12
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 150, 50);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(225.f), float32(495.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup roof platform #1, entity 13
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat4.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 350, 100);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(300.f), float32(625.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform used as you return from the door, entity 14
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 150, 50);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(500.f), float32(480.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup wall used to block player from skiping intro, entity 15
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 400);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(700.f), float32(275.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup wall used right after the intro, entity 16
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 400);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(800.f), float32(400.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform used right after the main sequence, entity 17
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat4.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 50);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(800.f), float32(100.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
	}
	//setup platform to use as falling door #1, entity 18
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 200);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_kinematicBody;
		tempDef.position.Set(float32(950.f), float32(100.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 24), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup platform to use as falling door #2, entity 19
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 200);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_kinematicBody;
		tempDef.position.Set(float32(1150.f), float32(150.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 24), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup platform to use as falling door #3, entity 20
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 200);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_kinematicBody;
		tempDef.position.Set(float32(1150.f), float32(200.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 24), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup platform used right after the falling rock sequence, entity 21
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat4.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 50);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1350.f), float32(50.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
	}
	//setup platform #2 used right after the falling rock sequence, entity 22
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat4.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 50);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1600.f), float32(125.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
	}
	//setup platform #3 used right after the falling rock sequence, entity 23
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 50);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_kinematicBody;
		tempDef.position.Set(float32(1780.f), float32(125.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 24), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup platform #4 used right after the falling rock sequence, entity 24
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat4.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 50);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1620.f), float32(325.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
	}
	//setup platform #5 used right after the falling rock sequence, entity 25
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat4.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 250, 50);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1300.f), float32(295.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
	}
	//setup platform #6 used right after the falling rock sequence, entity 26
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat4.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 20);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1300.f), float32(410.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
	}
	//setup platform #7 used right after the falling rock sequence, entity 27
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 20);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_kinematicBody;
		tempDef.position.Set(float32(1375.f), float32(500.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
	}
	//setup platform #8 used right after the falling rock sequence, entity 28
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat4.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 50);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1750.f), float32(480.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
	}
	//setup wall used right after the second sequence, entity 29
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 400);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1900.f), float32(325.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup wall #2 used right after the second sequence, entity 30
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 400);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(2000.f), float32(450.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform used right after the third sequence, entity 31
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat4.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 50);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1980.f), float32(150.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
	}
	//Time remaining hud entity 32
	{
		auto entity = ECS::CreateEntity();
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		std::string fileName = "timemeter.png";

		auto& animController = ECS::GetComponent<AnimationController>(entity);

		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		animController.SetActiveAnim(0);
		auto& full = animController.GetAnimation(0);
		full.AddFrame(vec2(0.f, 614.f), vec2(584.f, 0.f));
		full.SetRepeating(true);
		full.SetSecPerFrame(0.1f);

		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		auto& minus1 = animController.GetAnimation(1);
		minus1.AddFrame(vec2(637.f, 614.f), vec2(1221.f, 0.f));
		minus1.SetRepeating(true);
		minus1.SetSecPerFrame(0.1f);

		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		auto& minus2 = animController.GetAnimation(2);
		minus2.AddFrame(vec2(1258, 614), vec2(1842, 0));
		minus2.SetRepeating(true);
		minus2.SetSecPerFrame(0.1f);

		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		auto& minus3 = animController.GetAnimation(3);
		minus3.AddFrame(vec2(1883, 614), vec2(2467, 0));
		minus3.SetRepeating(true);
		minus3.SetSecPerFrame(0.1f);

		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		auto& minus4 = animController.GetAnimation(4);
		minus4.AddFrame(vec2(0, 1318), vec2(584, 704));
		minus4.SetRepeating(true);
		minus4.SetSecPerFrame(0.1f);

		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		auto& minus5 = animController.GetAnimation(5);
		minus5.AddFrame(vec2(656, 1318), vec2(1240, 704));
		minus5.SetRepeating(true);
		minus5.SetSecPerFrame(0.1f);

		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		auto& minus6 = animController.GetAnimation(6);
		minus6.AddFrame(vec2(1288, 1318), vec2(1872, 704));
		minus6.SetRepeating(true);
		minus6.SetSecPerFrame(0.1f);

		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		auto& minus7 = animController.GetAnimation(7);
		minus7.AddFrame(vec2(1898, 1318), vec2(2482, 704));
		minus7.SetRepeating(true);
		minus7.SetSecPerFrame(0.1f);

		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		auto& minus8 = animController.GetAnimation(8);
		minus8.AddFrame(vec2(2501, 1318), vec2(3085, 700));
		minus8.SetRepeating(true);
		minus8.SetSecPerFrame(0.1f);

		ECS::GetComponent<Transform>(entity).SetPosition(ECS::GetComponent<PhysicsBody>(1).GetPosition().x, ECS::GetComponent<PhysicsBody>(1).GetPosition().y, 99);
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 35, 45, true, &animController);

		//Sets up identifier
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Time Left UI");
	}

	//Hp HUD entity 33
	{
		auto entity = ECS::CreateEntity();
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		std::string fileName = "health.png";

		auto& animController = ECS::GetComponent<AnimationController>(entity);

		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		animController.SetActiveAnim(0);
		auto& full = animController.GetAnimation(0);
		full.AddFrame(vec2(0.f, 207.f), vec2(818.f, 0.f));
		full.SetRepeating(true);
		full.SetSecPerFrame(0.1f);

		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		auto& minus1 = animController.GetAnimation(1);
		minus1.AddFrame(vec2(0.f, 440.f), vec2(818.f, 233.f));
		minus1.SetRepeating(true);
		minus1.SetSecPerFrame(0.1f);

		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		auto& minus2 = animController.GetAnimation(2);
		minus2.AddFrame(vec2(0, 680), vec2(818, 473));
		minus2.SetRepeating(true);
		minus2.SetSecPerFrame(0.1f);

		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		auto& minus3 = animController.GetAnimation(3);
		minus3.AddFrame(vec2(0, 920), vec2(818, 713));
		minus3.SetRepeating(true);
		minus3.SetSecPerFrame(0.1f);

		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		auto& minus4 = animController.GetAnimation(4);
		minus4.AddFrame(vec2(970, 270), vec2(1788, 0));
		minus4.SetRepeating(true);
		minus4.SetSecPerFrame(0.1f);

		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		auto& minus5 = animController.GetAnimation(5);
		minus5.AddFrame(vec2(970, 484), vec2(1788, 227));
		minus5.SetRepeating(true);
		minus5.SetSecPerFrame(0.1f);

		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		auto& minus6 = animController.GetAnimation(6);
		minus6.AddFrame(vec2(970, 682), vec2(1788, 475));
		minus6.SetRepeating(true);
		minus6.SetSecPerFrame(0.1f);

		ECS::GetComponent<Transform>(entity).SetPosition(ECS::GetComponent<PhysicsBody>(1).GetPosition().x, ECS::GetComponent<PhysicsBody>(1).GetPosition().y, 99);
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 40, true, &animController);

		//Sets up identifier
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(entity, bitHolder, "HP UI");
	}

	//setup platform for falling rtock section #2, #1, entity 34
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 160);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_kinematicBody;
		tempDef.position.Set(float32(2170.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 24), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup platform for falling rock section #2, #2, entity 35
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat2.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 120, 50);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_kinematicBody;
		tempDef.position.Set(float32(2370.f), float32(100.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 24), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup platform for falling rtock section #2, #3, entity 36
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat2.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 60, 25);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_kinematicBody;
		tempDef.position.Set(float32(2450.f), float32(150.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 24), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup platform for falling rtock section #2, #4 , entity 37
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 120, 50);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_kinematicBody;
		tempDef.position.Set(float32(2600.f), float32(210.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 24), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup platform for break in falling rock #2 , entity 38
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat2.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 80, 50);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(2030.f), float32(340.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 24), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup platform for falling rtock section #2, #5, entity 39
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 90, 38);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_kinematicBody;
		tempDef.position.Set(float32(2520.f), float32(305.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 24), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup platform for falling rtock section #2, #6, entity 40
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat4.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 90, 38);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_kinematicBody;
		tempDef.position.Set(float32(2300.f), float32(340.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 24), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup platform for falling rtock section #2, #7, entity 41
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat2.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 90, 38);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_kinematicBody;
		tempDef.position.Set(float32(2180.f), float32(360.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 24), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup platform for break #2 in falling rock #2 , entity 42
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat2.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 80, 50);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_kinematicBody;
		tempDef.position.Set(float32(2770.f), float32(480.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 24), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup platform for break #2 in falling rock #2 , entity 43
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 60, 300);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(2700.f), float32(750.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 24), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup platform for break #2 in falling rock #2 , entity 44
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat2.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 50);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(3000.f), float32(620.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 24), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup platform for falling rock section #3, #1, entity 45
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 200);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_kinematicBody;
		tempDef.position.Set(float32(2875.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 24), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup platform for falling rock section #3, #1, entity 46
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 200);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_kinematicBody;
		tempDef.position.Set(float32(2875.f), float32(400.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 24), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup platform for falling rock section #3, #1, entity 47
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 200);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_kinematicBody;
		tempDef.position.Set(float32(2875.f), float32(800.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 24), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup platform for falling rock section #3, #1, entity 48
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 200);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_kinematicBody;
		tempDef.position.Set(float32(2875.f), float32(1200.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 24), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup goal ent 49
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "klockclock.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 35, 35);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 51.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_kinematicBody;
		tempDef.position.Set(float32(3050.f), float32(680.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 8), float(tempSpr.GetHeight() - 4),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//50
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PlayerData>(entity);

		ECS::GetComponent<PlayerData>(entity).Health = 4;

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "L1Enemy.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);

		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		animController.SetActiveAnim(0);
		auto& anim = animController.GetAnimation(0);
		//Walking right animation

		//robson fuck you, you fucking cuck. I bet if you ever get a significant other they will have an onlyfans. I hope you spill dorito dust on your fur suit sonicfox looks ass sounding ass being ass mother fucker. Platkop kaffir gaan eet jo ma se poss.
		anim.AddFrame(vec2(0.f, 642.f), vec2(716.f, 0.f));
		anim.AddFrame(vec2(716.f, 642.f), vec2(1432.f, 0.f));
		anim.AddFrame(vec2(1432.f, 642.f), vec2(2148.f, 0.f));
		anim.AddFrame(vec2(2148.f, 642.f), vec2(2864.f, 0.f));
		anim.AddFrame(vec2(0.f, 1284.f), vec2(716.f, 642.f));
		anim.AddFrame(vec2(716.f, 1284.f), vec2(1432.f, 642.f));
		anim.AddFrame(vec2(1432.f, 1284.f), vec2(2148.f, 642.f));
		anim.AddFrame(vec2(2148.f, 1284.f), vec2(2864.f, 642.f));

		anim.SetRepeating(true);
		anim.SetSecPerFrame(0.1f);

		//Sets up components
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 35, 45, true, &animController);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(550.f, 0.f, 49.f));

		//Grabs reference to various components
		//Sets up components

		//ECS::GetComponent<PlayerData>(entity).Health = 6;
		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(630.f), float32(267.f));
		tempDef.fixedRotation = true;

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 16), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		ECS::GetComponent<PhysicsBody>(entity).GetBody()->GetFixtureList()->SetUserData((void*)11);
		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//51
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PlayerData>(entity);

		ECS::GetComponent<PlayerData>(entity).Health = 4;

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "L1Enemy.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);

		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		animController.SetActiveAnim(0);
		auto& anim = animController.GetAnimation(0);
		//Walking right animation

		//robson fuck you, you fucking cuck. I bet if you ever get a significant other they will have an onlyfans. I hope you spill dorito dust on your fur suit sonicfox looks ass sounding ass being ass mother fucker. Platkop kaffir gaan eet jo ma se poss.
		anim.AddFrame(vec2(0.f, 642.f), vec2(716.f, 0.f));
		anim.AddFrame(vec2(716.f, 642.f), vec2(1432.f, 0.f));
		anim.AddFrame(vec2(1432.f, 642.f), vec2(2148.f, 0.f));
		anim.AddFrame(vec2(2148.f, 642.f), vec2(2864.f, 0.f));
		anim.AddFrame(vec2(0.f, 1284.f), vec2(716.f, 642.f));
		anim.AddFrame(vec2(716.f, 1284.f), vec2(1432.f, 642.f));
		anim.AddFrame(vec2(1432.f, 1284.f), vec2(2148.f, 642.f));
		anim.AddFrame(vec2(2148.f, 1284.f), vec2(2864.f, 642.f));

		anim.SetRepeating(true);
		anim.SetSecPerFrame(0.1f);

		//Sets up components
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 35, 45, true, &animController);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(550.f, 0.f, 49.f));

		//Grabs reference to various components
		//Sets up components

		//ECS::GetComponent<PlayerData>(entity).Health = 6;
		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(10.f), float32(452.f));
		tempDef.fixedRotation = true;

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 16), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		ECS::GetComponent<PhysicsBody>(entity).GetBody()->GetFixtureList()->SetUserData((void*)11);
		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//52
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PlayerData>(entity);

		ECS::GetComponent<PlayerData>(entity).Health = 4;

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "L1Enemy.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);

		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		animController.SetActiveAnim(0);
		auto& anim = animController.GetAnimation(0);
		//Walking right animation

		//robson fuck you, you fucking cuck. I bet if you ever get a significant other they will have an onlyfans. I hope you spill dorito dust on your fur suit sonicfox looks ass sounding ass being ass mother fucker. Platkop kaffir gaan eet jo ma se poss.
		anim.AddFrame(vec2(0.f, 642.f), vec2(716.f, 0.f));
		anim.AddFrame(vec2(716.f, 642.f), vec2(1432.f, 0.f));
		anim.AddFrame(vec2(1432.f, 642.f), vec2(2148.f, 0.f));
		anim.AddFrame(vec2(2148.f, 642.f), vec2(2864.f, 0.f));
		anim.AddFrame(vec2(0.f, 1284.f), vec2(716.f, 642.f));
		anim.AddFrame(vec2(716.f, 1284.f), vec2(1432.f, 642.f));
		anim.AddFrame(vec2(1432.f, 1284.f), vec2(2148.f, 642.f));
		anim.AddFrame(vec2(2148.f, 1284.f), vec2(2864.f, 642.f));

		anim.SetRepeating(true);
		anim.SetSecPerFrame(0.1f);

		//Sets up components
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 35, 45, true, &animController);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(550.f, 0.f, 49.f));

		//Grabs reference to various components
		//Sets up components

		//ECS::GetComponent<PlayerData>(entity).Health = 6;
		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(1560.f), float32(367.f));
		tempDef.fixedRotation = true;

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 16), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		ECS::GetComponent<PhysicsBody>(entity).GetBody()->GetFixtureList()->SetUserData((void*)11);
		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//Makes the camera focus on the main player
	ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
}

void Level2Scene::Update()
{
	std::cout << "X: " << ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x << '\t' << "Y: " << ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().y << '\n';

	ECS::GetComponent<PlayerData>(1).CurrentScene = 2;

	ECS::GetComponent<Transform>(32).SetPosition(ECS::GetComponent<HorizontalScroll>(2).GetCam()->GetPosition().x - 280, ECS::GetComponent<VerticalScroll>(2).GetCam()->GetPosition().y + 150, 99);
	ECS::GetComponent<Transform>(33).SetPosition(ECS::GetComponent<HorizontalScroll>(2).GetCam()->GetPosition().x - 220, ECS::GetComponent<VerticalScroll>(2).GetCam()->GetPosition().y + 200, 99);

	if (ECS::GetComponent<PlayerData>(1).isAttacking)
	{
		std::cout << "BRUH";

		createdint = 1;

		auto entity = ECS::CreateEntity();
		tempent = entity;
		//std::cout << entity << " " << tempent;
		//Add components
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 98.f));

		std::string fileName = "floatplatform.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20);
		//Grabs reference to various components
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		if (ECS::GetComponent<PlayerData>(1).facingLeft)
			tempDef.position.Set(float32(ECS::GetComponent<PhysicsBody>(1).GetPosition().x - 15), float32(ECS::GetComponent<PhysicsBody>(1).GetPosition().y));
		else
			tempDef.position.Set(float32(ECS::GetComponent<PhysicsBody>(1).GetPosition().x + 15), float32(ECS::GetComponent<PhysicsBody>(1).GetPosition().y));

		tempDef.fixedRotation = true;
		tempDef.gravityScale = 0;

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(1), float(1),
			vec2(0.f, 0.f), true, true);

		//fixture definition
		b2PolygonShape polygonShape;
		b2FixtureDef myFixtureDef;
		if (ECS::GetComponent<PlayerData>(1).facingLeft)
		{
			ECS::GetComponent<AnimationController>(1).SetActiveAnim(7);
			polygonShape.SetAsBox(35.f, 20.f, b2Vec2(0.f, 0.f), 0);
		}
		else
		{
			ECS::GetComponent<AnimationController>(1).SetActiveAnim(3);
			polygonShape.SetAsBox(35.f, 20.f, b2Vec2(0.f, 0.f), 0);
		}

		myFixtureDef.shape = &polygonShape;
		myFixtureDef.density = 0;
		myFixtureDef.isSensor = 1;
		//myFixtureDef.friction = 1.f;

		myFixtureDef.isSensor = true;
		b2Fixture* footSensorFixture = tempPhysBody.GetBody()->CreateFixture(&myFixtureDef);
		footSensorFixture->SetUserData((void*)7);

		//Sets up the identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "da hitbox");
		std::cout << entity << "\n" << tempent << "\n";
	}
	else if (ECS::GetComponent<PlayerData>(1).TimeSinceAtt <= 0 && createdint == 1)
	{
		ECS::DestroyEntity(tempent);
		std::cout << "Destroyed ent";

		ECS::GetComponent<PlayerData>(1).TimeSinceAtt = 0.4f;
		createdint = 0;
	}

	//Time slow resource ui for scene 1
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime < 1)
		ECS::GetComponent<AnimationController>(32).SetActiveAnim(0);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 1)
		ECS::GetComponent<AnimationController>(32).SetActiveAnim(1);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 2)
		ECS::GetComponent<AnimationController>(32).SetActiveAnim(2);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 3)
		ECS::GetComponent<AnimationController>(32).SetActiveAnim(3);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 4)
		ECS::GetComponent<AnimationController>(32).SetActiveAnim(4);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 5)
		ECS::GetComponent<AnimationController>(32).SetActiveAnim(5);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 6)
		ECS::GetComponent<AnimationController>(32).SetActiveAnim(6);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 7)
		ECS::GetComponent<AnimationController>(32).SetActiveAnim(7);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 8)
		ECS::GetComponent<AnimationController>(32).SetActiveAnim(8);

	//Updates the HP bar ui
	int i = ECS::GetComponent<PlayerData>(1).Health;
	switch (i)
	{
	case 0:	ECS::GetComponent<AnimationController>(33).SetActiveAnim(6);
		break;
	case 1: ECS::GetComponent<AnimationController>(33).SetActiveAnim(5);
		break;
	case 2: ECS::GetComponent<AnimationController>(33).SetActiveAnim(4);
		break;
	case 3: ECS::GetComponent<AnimationController>(33).SetActiveAnim(3);
		break;
	case 4: ECS::GetComponent<AnimationController>(33).SetActiveAnim(2);
		break;
	case 5: ECS::GetComponent<AnimationController>(33).SetActiveAnim(1);
		break;
	case 6: ECS::GetComponent<AnimationController>(33).SetActiveAnim(0);
		break;
	}

	//background scrolls with the camera
	ECS::GetComponent<Transform>(0).SetPositionX((ECS::GetComponent<HorizontalScroll>(2).GetCam()->GetPosition().x / 8) + 2800);
	ECS::GetComponent<Transform>(0).SetPositionY((ECS::GetComponent<VerticalScroll>(2).GetCam()->GetPosition().y / 1.2));

	//respawn mechanic
	if (ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().y < -50) {
		ECS::GetComponent<PlayerData>(1).Health--;
		if (ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x < 800) { //checkpint 1
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetTransform(b2Vec2(0, 150), 0);
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(0.f, 0.f));
		}
		else if (ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x < 1350) { //checkpoint 2
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetTransform(b2Vec2(850, 150), 0);
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(0.f, 0.f));
		}
		else if (ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x < 2000) { //checkpoint 2
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetTransform(b2Vec2(1350, 150), 0);
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(0.f, 0.f));
		}
		else if (ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x < 3000) { //checkpoint 2
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetTransform(b2Vec2(2050, 200), 0);
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(0.f, 0.f));
		}
	}
	//slowed down time check
	if (ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).isSlowed)
		platAcc = 0.1f;
	else platAcc = 1.0f;

	//platform 4 movements
	if (ECS::GetComponent<PhysicsBody>(4).GetBody()->GetPosition().y < 0 || ECS::GetComponent<PhysicsBody>(4).GetBody()->GetPosition().y > 150)
		plat4MoveSpeed = -plat4MoveSpeed;

	ECS::GetComponent<PhysicsBody>(4).GetBody()->SetLinearVelocity(b2Vec2(0.f, plat4MoveSpeed * platAcc));

	//entity 6 door close
	if (ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x > 235 && ECS::GetComponent<PhysicsBody>(6).GetBody()->GetPosition().y > 250)
		plat6MoveSpeed = -60.f;
	else if (ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x < 235 && ECS::GetComponent<PhysicsBody>(6).GetBody()->GetPosition().y < 400)
		plat6MoveSpeed = 30.f;
	else
		plat6MoveSpeed = 0.f;
	ECS::GetComponent<PhysicsBody>(6).GetBody()->SetLinearVelocity(b2Vec2(0.f, plat6MoveSpeed * platAcc));

	//entity 18, 19 & 20 (falling rocks)
	ECS::GetComponent<PhysicsBody>(18).GetBody()->SetLinearVelocity(b2Vec2(0.f, plat1819MoveSpeed * platAcc));
	ECS::GetComponent<PhysicsBody>(19).GetBody()->SetLinearVelocity(b2Vec2(0.f, plat1819MoveSpeed * platAcc));
	ECS::GetComponent<PhysicsBody>(20).GetBody()->SetLinearVelocity(b2Vec2(0.f, plat1819MoveSpeed * platAcc));

	if (ECS::GetComponent<PhysicsBody>(18).GetBody()->GetPosition().y < -400)
		ECS::GetComponent<PhysicsBody>(18).GetBody()->SetTransform(b2Vec2(950, 800), 0.f);
	if (ECS::GetComponent<PhysicsBody>(19).GetBody()->GetPosition().y < -400)
		ECS::GetComponent<PhysicsBody>(19).GetBody()->SetTransform(b2Vec2(1050, 800), 0.f);
	if (ECS::GetComponent<PhysicsBody>(20).GetBody()->GetPosition().y < -400)
		ECS::GetComponent<PhysicsBody>(20).GetBody()->SetTransform(b2Vec2(1150, 800), 0.f);

	//platform 23 movements
	if (ECS::GetComponent<PhysicsBody>(23).GetBody()->GetPosition().y < 100 || ECS::GetComponent<PhysicsBody>(23).GetBody()->GetPosition().y > 350)
		plat23MoveSpeed = -plat23MoveSpeed;
	ECS::GetComponent<PhysicsBody>(23).GetBody()->SetLinearVelocity(b2Vec2(0.f, plat23MoveSpeed * platAcc));
	//platform 27 movements
	if (ECS::GetComponent<PhysicsBody>(27).GetBody()->GetPosition().x < 1375 || ECS::GetComponent<PhysicsBody>(27).GetBody()->GetPosition().x > 1600)
		plat27MoveSpeed = -plat27MoveSpeed;
	ECS::GetComponent<PhysicsBody>(27).GetBody()->SetLinearVelocity(b2Vec2(plat27MoveSpeed * platAcc, 0.f));

	//really cool falling rock section
	ECS::GetComponent<PhysicsBody>(34).GetBody()->SetLinearVelocity(b2Vec2(0.f, plat34MoveSpeed * platAcc));
	ECS::GetComponent<PhysicsBody>(35).GetBody()->SetLinearVelocity(b2Vec2(0.f, plat34MoveSpeed * platAcc));
	ECS::GetComponent<PhysicsBody>(36).GetBody()->SetLinearVelocity(b2Vec2(0.f, plat34MoveSpeed * platAcc));
	ECS::GetComponent<PhysicsBody>(37).GetBody()->SetLinearVelocity(b2Vec2(0.f, plat34MoveSpeed * platAcc));
	ECS::GetComponent<PhysicsBody>(39).GetBody()->SetLinearVelocity(b2Vec2(0.f, plat34MoveSpeed * platAcc));
	ECS::GetComponent<PhysicsBody>(40).GetBody()->SetLinearVelocity(b2Vec2(0.f, plat34MoveSpeed * platAcc));
	ECS::GetComponent<PhysicsBody>(41).GetBody()->SetLinearVelocity(b2Vec2(0.f, plat34MoveSpeed * platAcc));
	ECS::GetComponent<PhysicsBody>(45).GetBody()->SetLinearVelocity(b2Vec2(0.f, plat45MoveSpeed * platAcc));
	ECS::GetComponent<PhysicsBody>(46).GetBody()->SetLinearVelocity(b2Vec2(0.f, plat45MoveSpeed * platAcc));
	ECS::GetComponent<PhysicsBody>(47).GetBody()->SetLinearVelocity(b2Vec2(0.f, plat45MoveSpeed * platAcc));
	ECS::GetComponent<PhysicsBody>(48).GetBody()->SetLinearVelocity(b2Vec2(0.f, plat45MoveSpeed * platAcc));

	if (ECS::GetComponent<PhysicsBody>(34).GetBody()->GetPosition().y < -400)
		ECS::GetComponent<PhysicsBody>(34).GetBody()->SetTransform(b2Vec2(2170, 1200), 0.f);
	if (ECS::GetComponent<PhysicsBody>(35).GetBody()->GetPosition().y < -400)
		ECS::GetComponent<PhysicsBody>(35).GetBody()->SetTransform(b2Vec2(2310, 1200), 0.f);
	if (ECS::GetComponent<PhysicsBody>(36).GetBody()->GetPosition().y < -400)
		ECS::GetComponent<PhysicsBody>(36).GetBody()->SetTransform(b2Vec2(2500, 1200), 0.f);
	if (ECS::GetComponent<PhysicsBody>(37).GetBody()->GetPosition().y < -400)
		ECS::GetComponent<PhysicsBody>(37).GetBody()->SetTransform(b2Vec2(2600, 1200), 0.f);
	if (ECS::GetComponent<PhysicsBody>(39).GetBody()->GetPosition().y < -400)
		ECS::GetComponent<PhysicsBody>(39).GetBody()->SetTransform(b2Vec2(2460, 1200), 0.f);
	if (ECS::GetComponent<PhysicsBody>(40).GetBody()->GetPosition().y < -400)
		ECS::GetComponent<PhysicsBody>(40).GetBody()->SetTransform(b2Vec2(2350, 1200), 0.f);
	if (ECS::GetComponent<PhysicsBody>(41).GetBody()->GetPosition().y < -400)
		ECS::GetComponent<PhysicsBody>(41).GetBody()->SetTransform(b2Vec2(2180, 1200), 0.f);
	if (ECS::GetComponent<PhysicsBody>(45).GetBody()->GetPosition().y < -400)
		ECS::GetComponent<PhysicsBody>(45).GetBody()->SetTransform(b2Vec2(2855, 1200), 0.f);
	if (ECS::GetComponent<PhysicsBody>(46).GetBody()->GetPosition().y < -400)
		ECS::GetComponent<PhysicsBody>(46).GetBody()->SetTransform(b2Vec2(2855, 1200), 0.f);
	if (ECS::GetComponent<PhysicsBody>(47).GetBody()->GetPosition().y < -400)
		ECS::GetComponent<PhysicsBody>(47).GetBody()->SetTransform(b2Vec2(2855, 1200), 0.f);
	if (ECS::GetComponent<PhysicsBody>(48).GetBody()->GetPosition().y < -400)
		ECS::GetComponent<PhysicsBody>(48).GetBody()->SetTransform(b2Vec2(2855, 1200), 0.f);

	if (ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x > 2730 && ECS::GetComponent<PhysicsBody>(42).GetBody()->GetPosition().y < 620)
		plat42MoveSpeed = 120.f;
	else if (ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x < 2700 && ECS::GetComponent<PhysicsBody>(42).GetBody()->GetPosition().y > 480)
		plat42MoveSpeed = -10.f;
	else
		plat42MoveSpeed = 0.f;
	ECS::GetComponent<PhysicsBody>(42).GetBody()->SetLinearVelocity(b2Vec2(0.f, plat42MoveSpeed));
	Level2Scene::EnemyUpdates();
}
void Level2Scene::EnemyUpdates()
{
	//enemy #1 | range and movement checks
	enemy1Distance = ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x - ECS::GetComponent<PhysicsBody>(50).GetBody()->GetPosition().x;
	if (enemy1Distance < 0 && enemy1Distance > -100 && ECS::GetComponent<PhysicsBody>(50).GetBody()->GetPosition().x > 565) //player on the left side of enemy
		ECS::GetComponent<PhysicsBody>(50).GetBody()->SetLinearVelocity(b2Vec2(-19 * platAcc, 0.f));
	else if (ECS::GetComponent<PhysicsBody>(50).GetBody()->GetPosition().x < 630) //enemy return to starting point
		ECS::GetComponent<PhysicsBody>(50).GetBody()->SetLinearVelocity(b2Vec2(10 * platAcc, 0.f));
	else
		ECS::GetComponent<PhysicsBody>(50).GetBody()->SetLinearVelocity(b2Vec2(0.f, 0.f));

	//enemy #2 | range and movement checks
	enemy2Distance = ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x - ECS::GetComponent<PhysicsBody>(51).GetBody()->GetPosition().x;
	if (enemy2Distance > 0 && enemy2Distance < 100 && ECS::GetComponent<PhysicsBody>(51).GetBody()->GetPosition().x < 70) //player on the right side of enemy
		ECS::GetComponent<PhysicsBody>(51).GetBody()->SetLinearVelocity(b2Vec2(19 * platAcc, 0.f));
	else if (ECS::GetComponent<PhysicsBody>(51).GetBody()->GetPosition().x > 10) //enemy return to starting point
		ECS::GetComponent<PhysicsBody>(51).GetBody()->SetLinearVelocity(b2Vec2(-10 * platAcc, 0.f));
	else
		ECS::GetComponent<PhysicsBody>(51).GetBody()->SetLinearVelocity(b2Vec2(0.f, 0.f));

	//enemy #3 | range and movement checks
	if (ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().y > 330)
		enemy3Distance = ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x - ECS::GetComponent<PhysicsBody>(52).GetBody()->GetPosition().x;
	if (enemy3Distance > 0 && enemy3Distance < 250 && ECS::GetComponent<PhysicsBody>(52).GetBody()->GetPosition().x < 1700) //player on the right side of enemy
		ECS::GetComponent<PhysicsBody>(52).GetBody()->SetLinearVelocity(b2Vec2(14 * platAcc, 0.f));
	else if (enemy3Distance < 0 && enemy3Distance > -250 && ECS::GetComponent<PhysicsBody>(52).GetBody()->GetPosition().x > 1545) //player on the left side of enemy
		ECS::GetComponent<PhysicsBody>(52).GetBody()->SetLinearVelocity(b2Vec2(-14 * platAcc, 0.f));
	else
		ECS::GetComponent<PhysicsBody>(52).GetBody()->SetLinearVelocity(b2Vec2(0.f, 0.f));
}