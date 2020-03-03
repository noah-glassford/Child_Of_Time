#include "Level1Scene.h"

//moving platforms shit i guess
float platformBSpeed = 5.f;
float platDSpeed;
float platXSpeed;

Level1Scene::Level1Scene(std::string name)
	: Scene(name)
{
	m_gravity = b2Vec2(float32(0.f), float32(-35.f));
	m_physicsWorld->SetGravity(m_gravity);
	m_physicsWorld->SetContactListener(&listener);
}

void Level1Scene::InitScene(float windowWidth, float windowHeight)
{


	
	//Allocates Register
	m_sceneReg = new entt::registry;

	//Attach the Register
	ECS::AttachRegister(m_sceneReg);

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
		std::string fileName = "Level1_BG.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 6400, 700);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(1000.f, 100.f, 1.f));

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "BackGround");
	}
#pragma region game_objects
	//Setup klock, entity 1
	{
		//Some JSON shit
		//auto klockAnimation = File::LoadJSON("klockmovement.json");

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
		//Sets up components
		std::string fileName = "spritesheet.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);

		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		animController.SetActiveAnim(0);
		auto& anim = animController.GetAnimation(0);
		//Walking right animation

		anim.AddFrame(vec2(0.f, 544.f), vec2(376.f, 0.f));
		anim.AddFrame(vec2(376.f, 544.f), vec2(752, 0.f));
		anim.AddFrame(vec2(752.f, 544.f), vec2(1128.f, 0.f));
		anim.AddFrame(vec2(376.f, 544.f), vec2(752, 0.f));
		anim.SetRepeating(true);
		anim.SetSecPerFrame(0.1f);

		//Walking left animation
		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		//animController.SetActiveAnim(1);
		auto& animation = animController.GetAnimation(1);

		animation.AddFrame(vec2(376.f, 544.f), vec2(0.f, 0.f));
		animation.AddFrame(vec2(752.f, 544.f), vec2(367.f, 0.f));
		animation.AddFrame(vec2(1128.f, 544.f), vec2(752.f, 0.f));
		animation.AddFrame(vec2(752.f, 544.f), vec2(376, 0.f));
		animation.SetRepeating(true);
		animation.SetSecPerFrame(0.1f);

		//Sets up components

		//Sets up components
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 35, 45, true, &animController);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(550.f, 0.f, 97.f));

		//Grabs reference to various components
		//Sets up components
		//ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 25);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 49.f));
		//Grabs reference to various components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetHeight() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(550.f), float32(220.f));
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
		polygonShape.SetAsBox(0.001f, 22.f, b2Vec2(25.f, 0.f), 0);
		myFixtureDef.isSensor = true;
		b2Fixture* RightSideFixture = tempPhysBody.GetBody()->CreateFixture(&myFixtureDef);
		RightSideFixture->SetUserData((void*)4);

		//Adds a fixture the left side of the body
		polygonShape.SetAsBox(0.001, 22.f, b2Vec2(-25.f, 0.f), 0);
		myFixtureDef.isSensor = true;
		b2Fixture* LeftSideFixture = tempPhysBody.GetBody()->CreateFixture(&myFixtureDef);
		LeftSideFixture->SetUserData((void*)5);

		//Sets up the identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Klock");
	}

	//Setup ai test enemy, entity 2
	{
		//Create new Entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PlayerData>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		//Sets up components
		std::string fileName = "Box.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 40);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 98.f));
		//ECS::GetComponent<PlayerData>(entity).Health = 3;
		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetHeight() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(550.f), float32(80.f));
		tempDef.fixedRotation = true;
		

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), true, 0.2f);

		tempBody->GetFixtureList()->SetUserData((void*)8);
		
		//Sets up the identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "ai enemy 2");
	}

#pragma endregion

#pragma region platforms
	//Setup the first platform ent 3
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "Level1_Platform.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 500);
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
		tempDef.position.Set(float32(50.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//Setup second platform ent 4 - c
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "front_grass_3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 300);
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
		tempDef.position.Set(float32(550.f), float32(40.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 16), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 2");
	}
	//setup third platform ent 5 - a
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "front_grass_3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 200);
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
		tempDef.position.Set(float32(200.f), float32(-50.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 10), float(tempSpr.GetHeight() - 10),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 3");
	}

	//setup fourth platform ent 6 - b
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "floatplatform.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 75, 20);
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
		tempDef.position.Set(float32(350.f), float32(110.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight() - 3),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}

	//setup fifth platform ent 7 - x
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "front_grass_2.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 300, 20);
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
		tempDef.position.Set(float32(350.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight() - 5),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 5");
	}
	//setup third platform ent 8 - g
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "front_grass_1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 300, 200);
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
		tempDef.position.Set(float32(1450.f), float32(70.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight() - 30),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 3");
	}
	//setup third platform ent 9 - f
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "front_grass_3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 300);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 51.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1700.f), float32(60.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight() - 24),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 3");
	}
	//setup fourth platform ent 10 - d
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "floatplatform.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 30);
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
		tempDef.position.Set(float32(500.f), float32(130.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}

	//setup third platform ent 11 - h
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "front_grass_3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 150, 300);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 51.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1900.f), float32(50.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight() - 24),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 3");
	}
	//setup fourth platform ent 12 - d
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "floatplatform.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 30);
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
		tempDef.position.Set(float32(1915.f), float32(125.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup third platform ent 13 - h
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "front_grass_3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 150, 300);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 51.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(2300.f), float32(50.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight() - 24),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 3");
	}

	//setup fourth platform ent 14 - k
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "floatplatform.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 30);
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
		tempDef.position.Set(float32(2500.f), float32(200.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup fourth platform ent 15 - j
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "floatplatform.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 30);
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
		tempDef.position.Set(float32(2650.f), float32(230.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup fourth platform ent 16 - j
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "floatplatform.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 30);
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
		tempDef.position.Set(float32(2800.f), float32(220.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup fourth platform ent 17 - j
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "floatplatform.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 30);
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
		tempDef.position.Set(float32(2950.f), float32(200.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup third platform ent 18 - h
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "front_grass_3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 150, 700);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 51.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(3150.f), float32(-75.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 30), float(tempSpr.GetHeight() - 24),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 3");
	}
	//setup third platform ent 19 - h
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "front_grass_1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 500, 300);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 51.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(3400.f), float32(-250.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 40), float(tempSpr.GetHeight() - 40),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 3");
	}
	//setup fourth platform ent 20
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "floatplatforms.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 30);
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
		tempDef.position.Set(float32(3250.f), float32(250.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 24), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup fourth platform ent 21
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "floatplatforms.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 30);
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
		tempDef.position.Set(float32(3350.f), float32(125.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 24), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
	//setup fourth platform ent 22
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "floatplatforms.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 30);
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
		tempDef.position.Set(float32(3250.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 24), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}

	//setup third platform ent 23
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "front_grass_3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 150, 700);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 51.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(3450.f), float32(325.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 20), float(tempSpr.GetHeight() - 24),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 3");
	}

	//setup third platform ent 24
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "front_grass_3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 450, 700);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 52.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_kinematicBody;
		tempDef.position.Set(float32(3800.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 20), float(tempSpr.GetHeight() - 24),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 3");
	}
	//setup fourth platform ent 25
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "floatplatform.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 30);
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
		tempDef.position.Set(float32(3550.f), float32(-150.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 24), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 4");
	}
#pragma endregion

#pragma region Camera&HUD
	//Main Camera
	{//Creates camera entity
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
		ECS::GetComponent<Camera>(entity).Zoom(-100);

		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<HorizontalScroll>(entity).SetOffset(15.f);
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetOffset(15.f);

		//Sets up identifier
		unsigned int bitHolder = EntityIdentifier::HoriScrollCameraBit() | EntityIdentifier::VertScrollCameraBit() | EntityIdentifier::CameraBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Main Scrolling Camera");
		ECS::SetIsMainCamera(entity, true);
	}
	
	//Putting HUD at bottom should prevent some problems gigalul
	//Time remaining hud
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
		minus2.AddFrame(vec2(1258,614), vec2(1842,0));
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

	//Hp HUD
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
#pragma endregion
	//Makes the camera focus on the main player
	ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));

	
}

void Level1Scene::Update()
{
	
	ECS::GetComponent<Transform>(27).SetPosition(ECS::GetComponent<HorizontalScroll>(26).GetCam()->GetPosition().x - 140, ECS::GetComponent<VerticalScroll>(26).GetCam()->GetPosition().y + 100, 99);
	ECS::GetComponent<Transform>(28).SetPosition(ECS::GetComponent<HorizontalScroll>(26).GetCam()->GetPosition().x - 100, ECS::GetComponent<VerticalScroll>(26).GetCam()->GetPosition().y + 150, 99);
	
	KlockAttack();
	PlatformMovement();

	

	//Time slow resource ui for scene 1
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime < 2)
		ECS::GetComponent<AnimationController>(27).SetActiveAnim(0);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 2)
		ECS::GetComponent<AnimationController>(27).SetActiveAnim(1);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 4)
		ECS::GetComponent<AnimationController>(27).SetActiveAnim(2);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 6)
		ECS::GetComponent<AnimationController>(27).SetActiveAnim(3);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 8)
		ECS::GetComponent<AnimationController>(27).SetActiveAnim(4);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 10)
		ECS::GetComponent<AnimationController>(27).SetActiveAnim(5);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 12)
		ECS::GetComponent<AnimationController>(27).SetActiveAnim(6);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 14)
		ECS::GetComponent<AnimationController>(27).SetActiveAnim(7);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 16)
		ECS::GetComponent<AnimationController>(27).SetActiveAnim(8);
	
	//Updates the HP bar ui
	int i = ECS::GetComponent<PlayerData>(1).Health;
	switch (i)
	{
	case 0:	ECS::GetComponent<AnimationController>(28).SetActiveAnim(6);
		break;
	case 1: ECS::GetComponent<AnimationController>(28).SetActiveAnim(5);
		break;
	case 2: ECS::GetComponent<AnimationController>(28).SetActiveAnim(4);
		break;
	case 3: ECS::GetComponent<AnimationController>(28).SetActiveAnim(3);
		break;
	case 4: ECS::GetComponent<AnimationController>(28).SetActiveAnim(2);
		break;
	case 5: ECS::GetComponent<AnimationController>(28).SetActiveAnim(1);
		break;
	case 6: ECS::GetComponent<AnimationController>(28).SetActiveAnim(0);
		break;
	}
	/*
	if (ECS::GetComponent<PlayerData>(1).Health == 6)
		ECS::GetComponent<AnimationController>(28).SetActiveAnim(0);
	else if (ECS::GetComponent<PlayerData>(1).Health == 5)
		ECS::GetComponent<AnimationController>(28).SetActiveAnim(1);
	else if (ECS::GetComponent<PlayerData>(1).Health == 4)
		ECS::GetComponent<AnimationController>(28).SetActiveAnim(2);
	else if (ECS::GetComponent<PlayerData>(1).Health == 3)
		ECS::GetComponent<AnimationController>(28).SetActiveAnim(3);
	else if (ECS::GetComponent<PlayerData>(1).Health == 2)
		ECS::GetComponent<AnimationController>(28).SetActiveAnim(4);
	else if (ECS::GetComponent<PlayerData>(1).Health == 1)
		ECS::GetComponent<AnimationController>(28).SetActiveAnim(5);
	else if (ECS::GetComponent<PlayerData>(1).Health == 0)
		ECS::GetComponent<AnimationController>(28).SetActiveAnim(6);
	*/

	//Makes the camera focus on the main player
	ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));


}

void Level1Scene::PlatformMovement()
{
	//platform B
	if (ECS::GetComponent<PhysicsBody>(6).GetPosition().y > 120)
		platformBSpeed = -5.f;
	else if (ECS::GetComponent<PhysicsBody>(6).GetPosition().y < 100)
		platformBSpeed = 5.f;

	ECS::GetComponent<PhysicsBody>(6).GetBody()->SetLinearVelocity(b2Vec2(0.f, platformBSpeed));

	ECS::GetComponent<PhysicsBody>(10).GetBody()->SetLinearVelocity(b2Vec2(platDSpeed, platformBSpeed));
	if (ECS::GetComponent<PhysicsBody>(10).GetPosition().x < 700)
		platDSpeed = 20.f;
	if (ECS::GetComponent<PhysicsBody>(10).GetPosition().x > 1250)
		platDSpeed = -20.f;

	if (ECS::GetComponent<PhysicsBody>(20).GetPosition().x < 3260)
		platXSpeed = 20.f;
	if (ECS::GetComponent<PhysicsBody>(20).GetPosition().x > 3340)
		platXSpeed = -20.f;

	if (ECS::GetComponent<PhysicsBody>(1).GetPosition().x > 3535 && ECS::GetComponent<PhysicsBody>(25).GetPosition().y < 300)
		ECS::GetComponent<PhysicsBody>(25).GetBody()->SetLinearVelocity(b2Vec2(0.f, 150.f));
	else
		ECS::GetComponent<PhysicsBody>(25).GetBody()->SetLinearVelocity(b2Vec2(0.f, 0.f));

	if (ECS::GetComponent<PhysicsBody>(1).GetPosition().x > 3535 && ECS::GetComponent<PhysicsBody>(24).GetPosition().x > 3730)
		ECS::GetComponent<PhysicsBody>(24).GetBody()->SetLinearVelocity(b2Vec2(-6.f, 0.f));
	else
		ECS::GetComponent<PhysicsBody>(24).GetBody()->SetLinearVelocity(b2Vec2(0.f, 0.f));

	if (ECS::GetComponent<PhysicsBody>(1).GetPosition().x > 1850 && ECS::GetComponent<PhysicsBody>(12).GetPosition().x < 2100)
		ECS::GetComponent<PhysicsBody>(12).GetBody()->SetLinearVelocity(b2Vec2(30.f, -platformBSpeed));
	else
		ECS::GetComponent<PhysicsBody>(12).GetBody()->SetLinearVelocity(b2Vec2(0.f, -platformBSpeed));

	ECS::GetComponent<PhysicsBody>(14).GetBody()->SetLinearVelocity(b2Vec2(0.f, platformBSpeed * 0.8));
	ECS::GetComponent<PhysicsBody>(15).GetBody()->SetLinearVelocity(b2Vec2(0.f, -platformBSpeed * 1.5));
	ECS::GetComponent<PhysicsBody>(16).GetBody()->SetLinearVelocity(b2Vec2(0.f, platformBSpeed * 1.2));
	ECS::GetComponent<PhysicsBody>(17).GetBody()->SetLinearVelocity(b2Vec2(0.f, -platformBSpeed * 2));

	ECS::GetComponent<PhysicsBody>(20).GetBody()->SetLinearVelocity(b2Vec2(platXSpeed, 0.f));
	ECS::GetComponent<PhysicsBody>(21).GetBody()->SetLinearVelocity(b2Vec2(-platXSpeed, 0.f));
	ECS::GetComponent<PhysicsBody>(22).GetBody()->SetLinearVelocity(b2Vec2(platXSpeed, 0.f));
}

void Level1Scene::KlockAttack()
{
	if (ECS::GetComponent<PlayerData>(1).isAttacking)
	{
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
			tempDef.position.Set(float32(ECS::GetComponent<PhysicsBody>(1).GetPosition().x - 45), float32(ECS::GetComponent<PhysicsBody>(1).GetPosition().y));
		else
			tempDef.position.Set(float32(ECS::GetComponent<PhysicsBody>(1).GetPosition().x + 45), float32(ECS::GetComponent<PhysicsBody>(1).GetPosition().y));

		tempDef.fixedRotation = true;
		tempDef.gravityScale = 0;

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(1), float(1),
			vec2(0.f, 0.f), true, true);

		//fixture definition 
		b2PolygonShape polygonShape;
		b2FixtureDef myFixtureDef;
		if (ECS::GetComponent<PlayerData>(1).facingLeft)
			polygonShape.SetAsBox(20.f, 20.f, b2Vec2(-45.f, 0.f), 0);
		else
			polygonShape.SetAsBox(20.f, 20.f, b2Vec2(45.f, 0.f), 0);

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

		ECS::GetComponent<PlayerData>(1).framesSinceAtt = 1;

	}
	
	
	if (ECS::GetComponent<PlayerData>(1).framesSinceAtt > 0 && ECS::GetComponent<PlayerData>(1).framesSinceAtt < 20)
	{
		ECS::GetComponent<PlayerData>(1).framesSinceAtt++;
		//std::cout << ECS::GetComponent<PlayerData>(1).framesSinceAtt;
	}
	else if (ECS::GetComponent<PlayerData>(1).framesSinceAtt == 20)
	{
		ECS::DestroyEntity(tempent);
		//std::cout << "Destroyed ent";
		ECS::GetComponent<PlayerData>(1).framesSinceAtt = 0;
	}
	
	//kills enemy
	if (ECS::GetComponent<PlayerData>(2).Health == 0 && tempbool == true)
	{
		std::cout << "Killed";
		ECS::GetComponent<PhysicsBody>(2).GetBody()->SetTransform(b2Vec2(-1000, 0), 0);
		tempbool = 0;
	}
	//Have this if statement for each of the enemies
}

