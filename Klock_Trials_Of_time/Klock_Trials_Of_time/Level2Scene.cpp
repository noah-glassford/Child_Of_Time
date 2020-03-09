#include "Level2Scene.h"

float platAcc;

float plat4MoveSpeed = 12;
float plat6MoveSpeed = 0;
float plat1819MoveSpeed = -120;

Level2Scene::Level2Scene(std::string name)
{
	m_gravity = b2Vec2(float32(0.f), float32(-35.f));
	m_physicsWorld->SetGravity(m_gravity);
	m_physicsWorld->SetContactListener(&listener);
}

void Level2Scene::InitScene(float windowWidth, float windowHeight)
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
		std::string fileName = "LV2BACK.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 6400, 700);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 200.f, 1.f));

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "BackGround");
	}

	//Setup klock, entity 1
		//Setup klock, entity 1
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
		tempDef.position.Set(float32(0.f), float32(150.f));
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
		ECS::GetComponent<Camera>(entity).Zoom(-125);

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
		tempDef.position.Set(float32(1150.f), float32(300.f));

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
		tempDef.position.Set(float32(1150.f), float32(500.f));

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
		tempDef.position.Set(float32(1400.f), float32(50.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
	}

	//Makes the camera focus on the main player
	ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
}

void Level2Scene::Update()
{
	std::cout << ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().x << '\n';

	//background scrolls with the camera
	ECS::GetComponent<Transform>(0).SetPositionX((ECS::GetComponent<HorizontalScroll>(2).GetCam()->GetPosition().x / 8) + 2800);
	ECS::GetComponent<Transform>(0).SetPositionY((ECS::GetComponent<VerticalScroll>(2).GetCam()->GetPosition().y / 1.4));

	//respawn mechanic
	if (ECS::GetComponent<PhysicsBody>(1).GetBody()->GetPosition().y < -50) {
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
	}
	//slowed down time check
	if (ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).isSlowed&& platAcc > 0.1f)
		platAcc -= 0.015f;
	else if (!ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).isSlowed&& platAcc < 1.f) platAcc += 0.02f;

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
		ECS::GetComponent<PhysicsBody>(18).GetBody()->SetTransform(b2Vec2(950, 900), 0.f);
	if (ECS::GetComponent<PhysicsBody>(19).GetBody()->GetPosition().y < -400)
		ECS::GetComponent<PhysicsBody>(19).GetBody()->SetTransform(b2Vec2(1050, 900), 0.f);
	if (ECS::GetComponent<PhysicsBody>(20).GetBody()->GetPosition().y < -400)
		ECS::GetComponent<PhysicsBody>(20).GetBody()->SetTransform(b2Vec2(1150, 900), 0.f);
}