#include "Level1Scene.h"

//stuff shtuff i guess
float platformSpeedMulti = 1.0f;
std::string newSign = "signtime.png";

//moving platforms shtuff i guess
float plat10MovementSpeedY;
float plat12MovementSpeedX = 8.f;
float plat15MovementSpeedX = 20.f;
float plat15MovementSpeedY;
bool plat15Up = false;
float plat25MovementSpeedY;
float plat34MovementSpeedX = 20.f;

//enemy stuff i guess
float enem1Distance;
float enem2Distance;
float enem3Distance;
float enem4Distance;
float enem5Distance;
float enem6Distance;
float enem7Distance;

Level1Scene::Level1Scene(std::string name)
	: Scene(name)
{
	m_gravity = b2Vec2(float32(0.f), float32(-60.f));
	m_physicsWorld->SetGravity(m_gravity);
	m_physicsWorld->SetContactListener(&listener);
}

void Level1Scene::InitScene(float windowWidth, float windowHeight)
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

#pragma region CORE_OBJECTS
	//Background entity, core entity #0
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "Level1_BG.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 6400, 1500);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(1000.f, 250.f, 2.f));

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "BackGround");
	}
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

	ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).canUseTimeSlow = false; // make sure this is called after the player is built

	//main camera entity, follows the player, core entity #2
	{
		//Creates camera entity, entity 2
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
#pragma endregion

#pragma region PLATFORM_OBJECTS
	//setup platform that you start on, entity #3
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "1_plat3.png";
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
		tempDef.position.Set(float32(0.f), float32(-300.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 24),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform that you start on #2, entity #4
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "1_plat3.png";
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
		tempDef.position.Set(float32(175.f), float32(-320.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 24),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform that you start on #3, entity #5
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "1_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 500);
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

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(340.f), float32(-275.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 24),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup jump tutorial sign, entity #6
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "signmove.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 120, 150);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(175.f, -25.f, 30.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform for the first platform challenge, entity #7
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "1_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 500);
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

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(620.f), float32(-320.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 24),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform for the first platform challenge #2, entity #8
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "1_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 400, 400);
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

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(820.f), float32(-320.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 22),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform for the first platform challenge #3, entity #9
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "1_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 500);
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

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1230.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 24),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup first moving platform, entity #10
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "1_plat1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 60, 20);
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
		tempDef.position.Set(float32(-2000.f), float32(-100.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 8), float(tempSpr.GetHeight() - 4),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform for the first platform challenge #4, entity #11
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "1_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 500);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 52.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1070.f), float32(-550.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 24),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup second moving platform, entity #12
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "1_plat1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 60, 20);
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
		tempDef.position.Set(float32(1250.f), float32(-330.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 8), float(tempSpr.GetHeight() - 4),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform for the first platform challenge #5, entity #13
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "1_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 500);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 52.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1470.f), float32(-520.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 24),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform for the first platform challenge #6, entity #14
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "1_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 500);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 53.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1620.f), float32(-440.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 24),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup third moving platform, entity #15
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "1_plat1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 60, 20);
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
		tempDef.position.Set(float32(1390.f), float32(-130.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 8), float(tempSpr.GetHeight() - 4),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform for the first platform challenge #7, entity #16
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "1_plat4.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 300, 175);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 53.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1590.f), float32(25.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 24),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform for the first platform challenge #7, entity #17
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "1_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 300, 700);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 54.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1800.f), float32(-200.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 34), float(tempSpr.GetHeight() - 28),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform for after the first platform challenge, entity #18
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "1_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 700);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 53.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1950.f), float32(-340.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 34), float(tempSpr.GetHeight() - 28),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform for after the first platform challenge #2, entity #19
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "1_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 700);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 53.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(2100.f), float32(-365.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 34), float(tempSpr.GetHeight() - 28),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform for after the first platform challenge #3, entity #20
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "1_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 700);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 53.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(2250.f), float32(-315.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 34), float(tempSpr.GetHeight() - 28),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup attack tutorial sign, entity #21
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "timecombat.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 120, 150);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(2250.f, 30.f, 50.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform for after the first platform challenge #4, entity #22
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "1_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 400, 700);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 60.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(2500.f), float32(-340.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 34), float(tempSpr.GetHeight() - 28),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//23
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
		tempDef.position.Set(float32(2375.f), float32(100.f));
		tempDef.fixedRotation = true;

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 16), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		ECS::GetComponent<PhysicsBody>(entity).GetBody()->GetFixtureList()->SetUserData((void*)11);
		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup the first enemy you encounter #2, entity #24
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PlayerData>(entity);

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

		ECS::GetComponent<PlayerData>(entity).Health = 4;

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
		tempDef.position.Set(float32(2575.f), float32(100.f));
		tempDef.fixedRotation = true;

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 16), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		ECS::GetComponent<PhysicsBody>(entity).GetBody()->GetFixtureList()->SetUserData((void*)12);
		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform from second skip, entity #25
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "1_plat1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 40);
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
		tempDef.position.Set(float32(-2000.f), float32(120.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 8), float(tempSpr.GetHeight() - 4),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform for after the first platform challenge #4, entity #26
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "1_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 700);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 53.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(2850.f), float32(-270.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 34), float(tempSpr.GetHeight() - 28),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform for after the first platform challenge #5, entity #27
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "1_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 700);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 53.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(3150.f), float32(-330.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 34), float(tempSpr.GetHeight() - 28),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform for after the first platform challenge #6, entity #28
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "1_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 700);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 53.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(3300.f), float32(-340.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 34), float(tempSpr.GetHeight() - 28),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform for after the first platform challenge #7, entity #29
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "1_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 700);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 54.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(3425.f), float32(-300.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 34), float(tempSpr.GetHeight() - 28),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup platform for after the first platform challenge #7, entity #30
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "1_plat3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 700);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 54.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(3600.f), float32(-260.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 34), float(tempSpr.GetHeight() - 28),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//setup shadow klock, entity #31
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "shadowklock.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 60);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 0.f, 54.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(3650.f), float32(110.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 34), float(tempSpr.GetHeight() - 28),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
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

	//setup moving platform, entity 34
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "1_plat1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 70, 35);
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
		tempDef.position.Set(float32(2860.f), float32(240.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 8), float(tempSpr.GetHeight() - 4),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}

	//setup goal ent 35
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
		tempDef.position.Set(float32(3600.f), float32(550.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 8), float(tempSpr.GetHeight() - 4),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//36
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
		tempDef.position.Set(float32(3150.f), float32(29.f));
		tempDef.fixedRotation = true;

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 16), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		ECS::GetComponent<PhysicsBody>(entity).GetBody()->GetFixtureList()->SetUserData((void*)13);
		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//37
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
		tempDef.position.Set(float32(4000.f), float32(0.f));
		tempDef.fixedRotation = true;

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 16), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		ECS::GetComponent<PhysicsBody>(entity).GetBody()->GetFixtureList()->SetUserData((void*)14);
		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//38
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
		tempDef.position.Set(float32(4000.f), float32(0.f));
		tempDef.fixedRotation = true;

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 16), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		ECS::GetComponent<PhysicsBody>(entity).GetBody()->GetFixtureList()->SetUserData((void*)15);
		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//39
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
		tempDef.position.Set(float32(4000.f), float32(0.f));
		tempDef.fixedRotation = true;

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 16), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		ECS::GetComponent<PhysicsBody>(entity).GetBody()->GetFixtureList()->SetUserData((void*)16);
		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}
	//40
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
		tempDef.position.Set(float32(4000.f), float32(0.f));
		tempDef.fixedRotation = true;

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 16), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		ECS::GetComponent<PhysicsBody>(entity).GetBody()->GetFixtureList()->SetUserData((void*)17);
		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}

	Sound2D _Music("Level1Music.mp3", "group1");
	_Music.play();

	//Makes the camera focus on the main player
	ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
}

void Level1Scene::Update()
{
	std::cout << "X: " << ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x << "\t\tY: " << ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().y << '\n';
	GeneralUpdates();
	PlatformMovement();
	EnemyUpdates();

	if (ECS::GetComponent<PlayerData>(23).Health == 0)
		ECS::GetComponent<PhysicsBody>(23).GetBody()->SetTransform(b2Vec2(99999, 999999), 0);

	if (ECS::GetComponent<PlayerData>(24).Health == 0)
		ECS::GetComponent<PhysicsBody>(24).GetBody()->SetTransform(b2Vec2(99999, 999999), 0);

	for (int i = 36; i <= 40; i++)
	{
		if (ECS::GetComponent<PlayerData>(i).Health == 0)
			ECS::GetComponent<PhysicsBody>(i).GetBody()->SetTransform(b2Vec2(99999, 999999), 0);
	}

	ECS::GetComponent<PlayerData>(1).CurrentScene = 1;

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

	ECS::GetComponent<Transform>(32).SetPosition(ECS::GetComponent<HorizontalScroll>(2).GetCam()->GetPosition().x - 280, ECS::GetComponent<VerticalScroll>(2).GetCam()->GetPosition().y + 150, 99);
	ECS::GetComponent<Transform>(33).SetPosition(ECS::GetComponent<HorizontalScroll>(2).GetCam()->GetPosition().x - 220, ECS::GetComponent<VerticalScroll>(2).GetCam()->GetPosition().y + 200, 99);

	//Time slow resource ui for scene 1
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime < 2)
		ECS::GetComponent<AnimationController>(32).SetActiveAnim(0);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 2)
		ECS::GetComponent<AnimationController>(32).SetActiveAnim(1);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 4)
		ECS::GetComponent<AnimationController>(32).SetActiveAnim(2);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 6)
		ECS::GetComponent<AnimationController>(32).SetActiveAnim(3);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 8)
		ECS::GetComponent<AnimationController>(32).SetActiveAnim(4);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 10)
		ECS::GetComponent<AnimationController>(32).SetActiveAnim(5);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 12)
		ECS::GetComponent<AnimationController>(32).SetActiveAnim(6);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 14)
		ECS::GetComponent<AnimationController>(32).SetActiveAnim(7);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 16)
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
}

void Level1Scene::GeneralUpdates()
{
	//background scrolls with the camera
	ECS::GetComponent<Transform>(0).SetPositionX((ECS::GetComponent<HorizontalScroll>(2).GetCam()->GetPosition().x / 8) + 2550);
	ECS::GetComponent<Transform>(0).SetPositionY((ECS::GetComponent<VerticalScroll>(2).GetCam()->GetPosition().y / 1.2) + 150);

	//checks if the player has the time slowed and adjusts accordingly
	if (ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).isSlowed)
		platformSpeedMulti = 0.1f;
	else
		platformSpeedMulti = 1.0f;

	//respawn mechanic
	if (ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().y < -500) {
		if (ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x < 950) { //checkpint 1
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetTransform(b2Vec2(0, 0), 0);
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(0.f, 0.f));
		}
		else if (ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x < 1350) { //checkpoint 2
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetTransform(b2Vec2(850, 150), 0);
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(0.f, -50.f));
		}
		else if (ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x < 4000) { //checkpoint 2
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetTransform(b2Vec2(1740, 160), 0);
			ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(0.f, 0.f));
		}
	}

	//reuns when the player reaches the swap point in the level, just ignore this it should all be fine
	if (ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x > 3550 && ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).canUseTimeSlow == false) {
		ECS::GetComponent<Sprite>(6).LoadSprite(newSign, 120, 150);
		ECS::GetComponent<Transform>(6).SetPositionX(865);
		ECS::GetComponent<Transform>(6).SetPositionY(-110);
		ECS::GetComponent<PhysicsBody>(1).GetBody()->SetTransform(b2Vec2(0, -39), 0);
		ECS::GetComponent<PhysicsBody>(1).GetBody()->SetLinearVelocity(b2Vec2(0.f, 0.f));
		ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).canUseTimeSlow = true;
		//new platforms
		ECS::GetComponent<PhysicsBody>(10).GetBody()->SetTransform(b2Vec2(1080, -100), 0);
		ECS::GetComponent<PhysicsBody>(25).GetBody()->SetTransform(b2Vec2(2000, 120), 0);
		ECS::GetComponent<PhysicsBody>(18).GetBody()->SetTransform(b2Vec2(2025, -230), 0);
		ECS::GetComponent<PhysicsBody>(19).GetBody()->SetTransform(b2Vec2(2150, 80), 0);
		ECS::GetComponent<PhysicsBody>(11).GetBody()->SetTransform(b2Vec2(1070, -350), 0);
		ECS::GetComponent<PhysicsBody>(20).GetBody()->SetTransform(b2Vec2(2300, -80), 0);
		//this line is broken and i don't know why the fuck it be like that, workin on fixing it
		//ECS::GetComponent<PhysicsBody>(21).GetBody()->SetTransform(b2Vec2(4000, -350), 0);
		ECS::GetComponent<PhysicsBody>(22).GetBody()->SetTransform(b2Vec2(1325, -150), 0);
		ECS::GetComponent<PhysicsBody>(26).GetBody()->SetTransform(b2Vec2(2550, -100), 0);
		ECS::GetComponent<PhysicsBody>(27).GetBody()->SetTransform(b2Vec2(3150, -60), 0);
		ECS::GetComponent<PhysicsBody>(28).GetBody()->SetTransform(b2Vec2(3300, -0), 0);
		ECS::GetComponent<PhysicsBody>(29).GetBody()->SetTransform(b2Vec2(3450, 60), 0);
		ECS::GetComponent<PhysicsBody>(30).GetBody()->SetTransform(b2Vec2(3600, 120), 0);

		ECS::GetComponent<PhysicsBody>(37).GetBody()->SetTransform(b2Vec2(335, -13), 0);
		ECS::GetComponent<PhysicsBody>(38).GetBody()->SetTransform(b2Vec2(1880, 159), 0);
		ECS::GetComponent<PhysicsBody>(39).GetBody()->SetTransform(b2Vec2(1790, 159), 0);
		ECS::GetComponent<PhysicsBody>(40).GetBody()->SetTransform(b2Vec2(3320, 360), 0);
	}
}

void Level1Scene::PlatformMovement()
{
	//platform 10 movements
	if (ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x > 900 && ECS::GetComponent<PhysicsBody>(10).GetBody()->GetPosition().y < 210)
		plat10MovementSpeedY = 30.f;
	else if (ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x < 900 && ECS::GetComponent<PhysicsBody>(10).GetBody()->GetPosition().y > -100)
		plat10MovementSpeedY = -30.f;
	else
		plat10MovementSpeedY = 0.f;
	ECS::GetComponent<PhysicsBody>(10).GetBody()->SetLinearVelocity(b2Vec2(0.f, plat10MovementSpeedY * platformSpeedMulti));

	//platform 12 movements
	if (ECS::GetComponent<PhysicsBody>(12).GetBody()->GetPosition().x < 1200 || ECS::GetComponent<PhysicsBody>(12).GetBody()->GetPosition().x > 1330)
		plat12MovementSpeedX = -plat12MovementSpeedX;
	ECS::GetComponent<PhysicsBody>(12).GetBody()->SetLinearVelocity(b2Vec2(plat12MovementSpeedX * platformSpeedMulti, 0.f));

	//platform 15 movements
	if (!plat15Up && ECS::GetComponent<PhysicsBody>(15).GetBody()->GetPosition().x > 1620)
		plat15MovementSpeedX = -plat15MovementSpeedX;
	else if (!plat15Up && ECS::GetComponent<PhysicsBody>(15).GetBody()->GetPosition().x < 1380) {
		plat15Up = true;
		plat15MovementSpeedY = -plat15MovementSpeedX;
		plat15MovementSpeedX = 0;
	}
	else if (plat15Up && ECS::GetComponent<PhysicsBody>(15).GetBody()->GetPosition().y > 75)
		plat15MovementSpeedY = -plat15MovementSpeedY;
	else if (plat15MovementSpeedY && ECS::GetComponent<PhysicsBody>(15).GetBody()->GetPosition().y < -180) {
		plat15Up = false;
		plat15MovementSpeedX = -plat15MovementSpeedY;
		plat15MovementSpeedY = 0;
	}
	ECS::GetComponent<PhysicsBody>(15).GetBody()->SetLinearVelocity(b2Vec2(plat15MovementSpeedX * platformSpeedMulti, plat15MovementSpeedY * platformSpeedMulti));

	//plat 25 movements
	if (ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x > 1800 && ECS::GetComponent<PhysicsBody>(25).GetBody()->GetPosition().y < 350)
		plat25MovementSpeedY = 60.f;
	else if (ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x < 1800 && ECS::GetComponent<PhysicsBody>(25).GetBody()->GetPosition().y > 120)
		plat25MovementSpeedY = -40.f;
	else
		plat25MovementSpeedY = 0.f;
	ECS::GetComponent<PhysicsBody>(25).GetBody()->SetLinearVelocity(b2Vec2(0.f, plat25MovementSpeedY * platformSpeedMulti));

	//platform 34 movements

	if (ECS::GetComponent<PhysicsBody>(34).GetBody()->GetPosition().x < 2700 || ECS::GetComponent<PhysicsBody>(34).GetBody()->GetPosition().x > 3000)
		plat34MovementSpeedX = -plat34MovementSpeedX;
	ECS::GetComponent<PhysicsBody>(34).GetBody()->SetLinearVelocity(b2Vec2(plat34MovementSpeedX * platformSpeedMulti, 0.f));
}

void Level1Scene::EnemyUpdates()
{
	//enemy #1 | range and movement checks
	enem1Distance = ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x - ECS::GetComponent<PhysicsBody>(23).GetBody()->GetPosition().x;
	if (enem1Distance > 0 && enem1Distance < 250 && ECS::GetComponent<PhysicsBody>(23).GetBody()->GetPosition().x < 2650) //player on the right side of enemy
		ECS::GetComponent<PhysicsBody>(23).GetBody()->SetLinearVelocity(b2Vec2(13 * platformSpeedMulti, 0.f));
	else if (enem1Distance < 0 && enem1Distance > -250 && ECS::GetComponent<PhysicsBody>(23).GetBody()->GetPosition().x > 2370) //player on the left side of enemy
		ECS::GetComponent<PhysicsBody>(23).GetBody()->SetLinearVelocity(b2Vec2(-13 * platformSpeedMulti, 0.f));
	else
		ECS::GetComponent<PhysicsBody>(23).GetBody()->SetLinearVelocity(b2Vec2(0.f, 0.f));

	//enemy #2 | range and movement checks
	enem2Distance = ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x - ECS::GetComponent<PhysicsBody>(24).GetBody()->GetPosition().x;
	if (enem2Distance > 0 && enem2Distance < 250 && ECS::GetComponent<PhysicsBody>(24).GetBody()->GetPosition().x < 2650) //player on the right side of enemy
		ECS::GetComponent<PhysicsBody>(24).GetBody()->SetLinearVelocity(b2Vec2(18 * platformSpeedMulti, 0.f));
	else if (enem2Distance < 0 && enem2Distance > -250 && ECS::GetComponent<PhysicsBody>(24).GetBody()->GetPosition().x > 2370) //player on the left side of enemy
		ECS::GetComponent<PhysicsBody>(24).GetBody()->SetLinearVelocity(b2Vec2(-18 * platformSpeedMulti, 0.f));
	else
		ECS::GetComponent<PhysicsBody>(24).GetBody()->SetLinearVelocity(b2Vec2(0.f, 0.f));

	//enemy #3 | range and movement checks
	enem3Distance = ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x - ECS::GetComponent<PhysicsBody>(36).GetBody()->GetPosition().x;
	if (enem3Distance > 0 && enem3Distance < 250 && ECS::GetComponent<PhysicsBody>(36).GetBody()->GetPosition().x < 3220) //player on the right side of enemy
		ECS::GetComponent<PhysicsBody>(36).GetBody()->SetLinearVelocity(b2Vec2(10 * platformSpeedMulti, 0.f));
	else if (enem3Distance < 0 && enem3Distance > -250 && ECS::GetComponent<PhysicsBody>(36).GetBody()->GetPosition().x > 3060) //player on the left side of enemy
		ECS::GetComponent<PhysicsBody>(36).GetBody()->SetLinearVelocity(b2Vec2(-10 * platformSpeedMulti, 0.f));
	else
		ECS::GetComponent<PhysicsBody>(36).GetBody()->SetLinearVelocity(b2Vec2(0.f, 0.f));

	//enemy #4| range and movement checks
	enem4Distance = ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x - ECS::GetComponent<PhysicsBody>(37).GetBody()->GetPosition().x;
	if (enem4Distance > 0 && enem4Distance < 250 && ECS::GetComponent<PhysicsBody>(37).GetBody()->GetPosition().x < 410) //player on the right side of enemy
		ECS::GetComponent<PhysicsBody>(37).GetBody()->SetLinearVelocity(b2Vec2(19 * platformSpeedMulti, 0.f));
	else if (enem4Distance < 0 && enem4Distance > -250 && ECS::GetComponent<PhysicsBody>(37).GetBody()->GetPosition().x > 280) //player on the left side of enemy
		ECS::GetComponent<PhysicsBody>(37).GetBody()->SetLinearVelocity(b2Vec2(-19 * platformSpeedMulti, 0.f));
	else
		ECS::GetComponent<PhysicsBody>(37).GetBody()->SetLinearVelocity(b2Vec2(0.f, 0.f));

	//enemy #5| range and movement checks
	enem5Distance = ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x - ECS::GetComponent<PhysicsBody>(38).GetBody()->GetPosition().x;
	if (enem5Distance > 0 && enem5Distance < 250 && ECS::GetComponent<PhysicsBody>(38).GetBody()->GetPosition().x < 1910) //player on the right side of enemy
		ECS::GetComponent<PhysicsBody>(38).GetBody()->SetLinearVelocity(b2Vec2(10 * platformSpeedMulti, 0.f));
	else if (enem5Distance < 0 && enem5Distance > -250 && ECS::GetComponent<PhysicsBody>(38).GetBody()->GetPosition().x > 1680) //player on the left side of enemy
		ECS::GetComponent<PhysicsBody>(38).GetBody()->SetLinearVelocity(b2Vec2(-10 * platformSpeedMulti, 0.f));
	else
		ECS::GetComponent<PhysicsBody>(38).GetBody()->SetLinearVelocity(b2Vec2(0.f, 0.f));

	//enemy #6| range and movement checks
	enem6Distance = ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x - ECS::GetComponent<PhysicsBody>(39).GetBody()->GetPosition().x;
	if (enem6Distance > 0 && enem6Distance < 250 && ECS::GetComponent<PhysicsBody>(39).GetBody()->GetPosition().x < 1910) //player on the right side of enemy
		ECS::GetComponent<PhysicsBody>(39).GetBody()->SetLinearVelocity(b2Vec2(11 * platformSpeedMulti, 0.f));
	else if (enem6Distance < 0 && enem6Distance > -250 && ECS::GetComponent<PhysicsBody>(39).GetBody()->GetPosition().x > 1680) //player on the left side of enemy
		ECS::GetComponent<PhysicsBody>(39).GetBody()->SetLinearVelocity(b2Vec2(-11 * platformSpeedMulti, 0.f));
	else
		ECS::GetComponent<PhysicsBody>(39).GetBody()->SetLinearVelocity(b2Vec2(0.f, 0.f));

	//enemy #7| range and movement checks
	enem7Distance = ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x - ECS::GetComponent<PhysicsBody>(40).GetBody()->GetPosition().x;
	if (enem7Distance > 0 && enem7Distance < 250 && ECS::GetComponent<PhysicsBody>(40).GetBody()->GetPosition().x < 3325) //player on the right side of enemy
		ECS::GetComponent<PhysicsBody>(40).GetBody()->SetLinearVelocity(b2Vec2(19 * platformSpeedMulti, 0.f));
	else if (enem7Distance < 0 && enem7Distance > -250 && ECS::GetComponent<PhysicsBody>(40).GetBody()->GetPosition().x > 3230) //player on the left side of enemy
		ECS::GetComponent<PhysicsBody>(40).GetBody()->SetLinearVelocity(b2Vec2(-19 * platformSpeedMulti, 0.f));
	else
		ECS::GetComponent<PhysicsBody>(40).GetBody()->SetLinearVelocity(b2Vec2(0.f, 0.f));
}