#include "MainMenu.h"


MainMenu::MainMenu(std::string name)
	:Scene(name)
{
}

void MainMenu::InitScene(float windowWidth, float windowHeight)
{
	
	
	m_sceneReg = new entt::registry;

	ECS::AttachRegister(m_sceneReg);

	float aspectRatio = windowWidth / windowHeight;

	//main camera entity, follows the player, core entity #0
	{

		auto entity = ECS::CreateEntity();
		EntityIdentifier::MainCamera(entity);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<Transform>(entity);

		vec4 temp = ECS::GetComponent<Camera>(entity).GetOrthoSize();
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);
		ECS::GetComponent<Camera>(entity).Zoom(-430);



		//Sets up identifier
		unsigned int bitHolder = EntityIdentifier::CameraBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Main Scrolling Camera");
		ECS::SetIsMainCamera(entity, true);
	}
	//hacky bullshit, will crash unless this is here, ent 1
	//By hacky bullshit I mean if this isn't a full player entity it crashes just don't open this shit aight ok cool
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
		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		auto& jumpRight = animController.GetAnimation(2);

		jumpRight.AddFrame(vec2(46, 1789), vec2(768, 922));
		jumpRight.AddFrame(vec2(818, 1794), vec2(1589, 923));
		jumpRight.AddFrame(vec2(1652, 1801), vec2(2538, 929));
		jumpRight.AddFrame(vec2(2483, 1801), vec2(3075, 933));
		jumpRight.SetRepeating(false);
		jumpRight.SetSecPerFrame(0.1f);

		//idle animation facing right
		animController.InitUVs(fileName);
		animController.AddAnimation(Animation());
		auto& idleRight = animController.GetAnimation(3);


		idleRight.AddFrame(vec2(97, 2703), vec2(689, 1839));
		idleRight.AddFrame(vec2(897, 2703), vec2(1489, 1848));
		idleRight.AddFrame(vec2(1697, 2703), vec2(1742, 1857));
		idleRight.AddFrame(vec2(2497, 2703), vec2(3089, 1866));
		idleRight.AddFrame(vec2(3297, 2703), vec2(3889, 1862));
		idleRight.AddFrame(vec2(4097, 2703), vec2(5489, 1844));
		idleRight.AddFrame(vec2(5697, 2703), vec2(6289, 1834));
		idleRight.SetRepeating(true);
		idleRight.SetSecPerFrame(0.1f);

		//Sets up components
		animController.SetActiveAnim(1);
		//Sets up components
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 35, 45, true, &animController);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(550.f, 0.f, 97.f));

		//Grabs reference to various components
		//Sets up components
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(999999.f, 9999999.f, 49.f));
		ECS::GetComponent<PlayerData>(entity).Health = 6;
		ECS::GetComponent<PlayerData>(entity).canUseTimeSlow = true;
		ECS::GetComponent<PlayerData>(entity).CurrentScene = 0;
		//Grabs reference to various components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetHeight() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(99999999999999999999.f), float32(9999999999999999.f));
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

	//Background entity, core entity #2
	{
	//Create new entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);

	//Sets up components
	std::string fileName = "MenuScreen.png";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1920, 1080);
	ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 2.f));

	unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
	ECS::SetUpIdentifier(entity, bitHolder, "BackGround");
	}
	//loading screen
		//Background entity, core entity #3
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "LoadingScreen1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1920, 1080);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 1.f));

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "BackGround");
	}
	Sound2D _Music("MenuSong.mp3", "group1");
		_Music.play();

}

void MainMenu::Update()
{

}
