#include "BossFightScene.h"

BossFightScene::BossFightScene(std::string name)
	: Scene(name)
{
	m_gravity = b2Vec2(float32(0.f), float32(-60.f));
	m_physicsWorld->SetGravity(m_gravity);
	m_physicsWorld->SetContactListener(&listener);
}


void BossFightScene::InitScene(float windowWidth, float windowHeight)
{

	//Allocates Register
	m_sceneReg = new entt::registry;

	//Attach the Register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;
	
#pragma region CORE_OBJECTS
//This region contains all the main gameplay objects
//Main Camera, entity 0
	{

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
		std::string fileName = "spritesheet_full.png";
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

		//Jumping while facing right
		auto& jumpRight = animController.GetAnimation(2);

		jumpRight.AddFrame(vec2(46,1789), vec2(768,922));
		jumpRight.AddFrame(vec2(818,1794), vec2(1589,923));
		jumpRight.AddFrame(vec2(1652,1801), vec2(2538,929));
		jumpRight.AddFrame(vec2(2483,1801), vec2(3075,933));
		jumpRight.SetRepeating(false);
		jumpRight.SetSecPerFrame(0.1f);

		//idle animation facing right
		auto& idleRight = animController.GetAnimation(3);

		idleRight.AddFrame(vec2(97,2703), vec2(689,1839));
		idleRight.AddFrame(vec2(897,2703), vec2(1489,1848));
		idleRight.AddFrame(vec2(1697,2703), vec2(1742,1857));
		idleRight.AddFrame(vec2(2497,2703), vec2(3089,1866));
		idleRight.AddFrame(vec2(3297,2703), vec2(3889,1862));
		idleRight.AddFrame(vec2(4097,2703), vec2(5489,1844));
		idleRight.AddFrame(vec2(5697,2703), vec2(6289,1834));
		idleRight.SetRepeating(true);
		idleRight.SetSecPerFrame(0.1f);

		//Sets up components

		//Sets up components
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 35, 45, true, &animController);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(550.f, 0.f, 97.f));

		//Grabs reference to various components
		//Sets up components
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 49.f));
		ECS::GetComponent<PlayerData>(entity).Health = 6;
		ECS::GetComponent<PlayerData>(entity).canUseTimeSlow = true;
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

	//Shadow Klock Entity 2
	{
		//Create new Entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<BossObject>(entity);

		//Sets up components
		std::string fileName = "Missing_Texture.jpg";

		//Sets up components
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 35, 45, true);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(550.f, 0.f, 97.f));

		//Grabs reference to various components
		//Sets up components
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 49.f));
		ECS::GetComponent<BossObject>(entity).EntityNumber = entity;
		//Grabs reference to various components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetHeight() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0.f), float32(70.f));
		tempDef.fixedRotation = true;
		tempDef.gravityScale = 0;

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), true, 1.5f);

		//Sets the body to hit klock if he touches it
		ECS::GetComponent<PhysicsBody>(1).GetBody()->GetFixtureList()->SetUserData((void*)8);
		

		//Sets up the identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Shadow Klock");
	}

	//Time remaining hud entity 3
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

	//Hp HUD entity 4
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

#pragma region Platforms
//This region contains all the platforms

	//ent 3
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 600, 50);
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
		tempDef.position.Set(float32(0.f), float32(-20.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}

	//ent 4
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName,50,400);
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
		tempDef.position.Set(float32(300.f), float32(150.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}

	//ent 5
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 600, 50);
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
		tempDef.position.Set(float32(0.f), float32(300.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}


	//ent 6
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "2_plat1.png";
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
		tempDef.position.Set(float32(-300.f), float32(150.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - 28), float(tempSpr.GetHeight() - 12),
			vec2(0.f, 0.f), false, 1.5f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Platform 1");
	}


#pragma endregion


	//Makes the camera focus on the main player
	ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
}

void BossFightScene::Update()
{
	//UI stuff
	ECS::GetComponent<Transform>(3).SetPosition(ECS::GetComponent<HorizontalScroll>(0).GetCam()->GetPosition().x - 260, ECS::GetComponent<VerticalScroll>(0).GetCam()->GetPosition().y + 150, 99);
	ECS::GetComponent<Transform>(4).SetPosition(ECS::GetComponent<HorizontalScroll>(0).GetCam()->GetPosition().x - 220, ECS::GetComponent<VerticalScroll>(0).GetCam()->GetPosition().y + 200, 99);

	
	
	//Attacking stuff
	if (createdint)
	{
		ECS::DestroyEntity(tempent);
		createdint = 0;
		std::cout << "deleting shit";
	}
	if (ECS::GetComponent<PlayerData>(1).isAttacking)
	{
		createdint = 1;
		auto entity = ECS::CreateEntity();
		tempent = entity;
		//std::cout << entity << " " << tempent;
		//Add components
		ECS::AttachComponent<Transform>(entity);

		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 98.f));

		//Grabs reference to various components
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(ECS::GetComponent<PhysicsBody>(1).GetPosition().x + 15), float32(ECS::GetComponent<PhysicsBody>(1).GetPosition().y));
		tempDef.fixedRotation = true;
		tempDef.gravityScale = 0;

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(1), float(1),
			vec2(0.f, 0.f), true, true);

		//fixture definition
		b2PolygonShape polygonShape;
		b2FixtureDef myFixtureDef;
		myFixtureDef.shape = &polygonShape;
		myFixtureDef.density = 0;
		myFixtureDef.isSensor = 1;
		//myFixtureDef.friction = 1.f;

		//Combat fixture
		polygonShape.SetAsBox(12.f, 12.f, b2Vec2(0.f, 35.f), 0);
		myFixtureDef.isSensor = true;
		b2Fixture* footSensorFixture = tempPhysBody.GetBody()->CreateFixture(&myFixtureDef);
		footSensorFixture->SetUserData((void*)7);

		//Sets up the identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "da hitbox");
		std::cout << entity << "\n" << tempent << "\n";
	}

	ECS::GetComponent<BossObject>(2).RunAI();
	
	//Time slow resource ui for scene 1
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime < 2)
		ECS::GetComponent<AnimationController>(3).SetActiveAnim(0);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 2)
		ECS::GetComponent<AnimationController>(3).SetActiveAnim(1);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 4)
		ECS::GetComponent<AnimationController>(3).SetActiveAnim(2);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 6)
		ECS::GetComponent<AnimationController>(3).SetActiveAnim(3);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 8)
		ECS::GetComponent<AnimationController>(3).SetActiveAnim(4);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 10)
		ECS::GetComponent<AnimationController>(3).SetActiveAnim(5);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 12)
		ECS::GetComponent<AnimationController>(3).SetActiveAnim(6);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 14)
		ECS::GetComponent<AnimationController>(3).SetActiveAnim(7);
	if (ECS::GetComponent<PlayerData>(1).UsedUpTime > 16)
		ECS::GetComponent<AnimationController>(3).SetActiveAnim(8);

	//Updates the HP bar ui
	int i = ECS::GetComponent<PlayerData>(1).Health;
	switch (i)
	{
	case 0:	ECS::GetComponent<AnimationController>(4).SetActiveAnim(6);
		break;
	case 1: ECS::GetComponent<AnimationController>(4).SetActiveAnim(5);
		break;
	case 2: ECS::GetComponent<AnimationController>(4).SetActiveAnim(4);
		break;
	case 3: ECS::GetComponent<AnimationController>(4).SetActiveAnim(3);
		break;
	case 4: ECS::GetComponent<AnimationController>(4).SetActiveAnim(2);
		break;
	case 5: ECS::GetComponent<AnimationController>(4).SetActiveAnim(1);
		break;
	case 6: ECS::GetComponent<AnimationController>(4).SetActiveAnim(0);
		break;
	}
	

}
