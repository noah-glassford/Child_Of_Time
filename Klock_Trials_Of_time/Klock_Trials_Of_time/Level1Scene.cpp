#include "Level1Scene.h"

Level1Scene::Level1Scene(std::string name)
	: Scene(name)
{
	m_gravity = b2Vec2(float32(0.f), float32(-70.f));
	m_physicsWorld->SetGravity(m_gravity);
}

void Level1Scene::InitScene(float windowWidth, float windowHeight)
{
	//Allocates Register
	m_sceneReg = new entt::registry;

	//Attach the Register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	//Background and Ground Object entity 0
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "Level1_BG.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 6400, 700);
		ECS::GetComponent<Sprite>(entity).SetSizeScale(0.1);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 1.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers half the sprite
			//Id type is environment
		float shrinkX = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;

		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(0.f), float32(50.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() / 4.f),
			vec2(0.f, (-tempSpr.GetHeight() / 20.f)*8.f), false);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "BackGround");
	}

	//Setup klock, entity 1
	{
		//Create new Entity
		auto entity = ECS::CreateEntity();
		EntityIdentifier::MainPlayer(entity);

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		//Sets up components 
		std::string fileName = "spriteSheet.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 28.5, 43.8);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 2.f));
		//Grabs reference to various components 


		

		//Sets up components
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 25);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 97.f));
		//Grabs reference to various components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetHeight() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-300.f), float32(50.f));
		tempDef.fixedRotation = true;
		

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), true);

		//fixture definition
		b2PolygonShape polygonShape;
		b2FixtureDef myFixtureDef;
		myFixtureDef.shape = &polygonShape;
<<<<<<< Updated upstream
		myFixtureDef.density = 10;
		//myFixtureDef.friction = 1.f;

=======
		myFixtureDef.density = 1.0;
		myFixtureDef.friction = 1.0f;
		tempBody->SetGravityScale(4);
>>>>>>> Stashed changes
		//Adds a fixture the size of the body
		polygonShape.SetAsBox(tempSpr.GetWidth(), tempSpr.GetHeight() / 2.f, b2Vec2(0, 0), 0);
		myFixtureDef.isSensor = true;
		b2Fixture* footSensorFixture = tempPhysBody.GetBody()->CreateFixture(&myFixtureDef);
		footSensorFixture->SetUserData((void*)3);

		//Sets up the identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Box1");
	}

	//Setup ai test enemy, entity 2
	{
		//Create new Entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		ECS::AttachComponent<PhysicsBody>(entity);
		//Sets up components
		std::string fileName = "Box.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 35);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 98.f));
		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetHeight() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(300.f), float32(80.f));
		tempDef.fixedRotation = true;

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), true);

		//fixture definition
		b2PolygonShape polygonShape;
		b2FixtureDef myFixtureDef;
		myFixtureDef.shape = &polygonShape;
		myFixtureDef.density = 4;
		myFixtureDef.friction = 1.f;
		tempBody->SetGravityScale(0);
		//Adds a fixture the size of the body
		polygonShape.SetAsBox(tempSpr.GetWidth(), tempSpr.GetHeight() / 2.f, b2Vec2(0, 0), 0);
		myFixtureDef.isSensor = true;
		b2Fixture* footSensorFixture = tempPhysBody.GetBody()->CreateFixture(&myFixtureDef);
		footSensorFixture->SetUserData((void*)3);

		//Sets up the identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Box1");
	}
	//Setup ai test enemy, entity 3
	{
		//Create new Entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		ECS::AttachComponent<PhysicsBody>(entity);
		//Sets up components
		std::string fileName = "Box.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 98.f));
		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetHeight() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-160.f), float32(80.f));
		tempDef.fixedRotation = true;

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), true);

		//fixture definition
		b2PolygonShape polygonShape;
		b2FixtureDef myFixtureDef;
		myFixtureDef.shape = &polygonShape;
		myFixtureDef.density = 4;
		myFixtureDef.friction = 1.f;
		tempBody->SetGravityScale(0);
		//Adds a fixture the size of the body
		polygonShape.SetAsBox(tempSpr.GetWidth(), tempSpr.GetHeight() / 2.f, b2Vec2(0, 0), 0);
		myFixtureDef.isSensor = true;
		b2Fixture* footSensorFixture = tempPhysBody.GetBody()->CreateFixture(&myFixtureDef);
		footSensorFixture->SetUserData((void*)3);

		//Sets up the identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Box1");
	}
	
	//Setup image for the platforms
	{
		//Create new Entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
	
		//Sets up components
		std::string fileName = "tut_lv.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 2500,200);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 98.f));
		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		
		//Sets up the identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Box1");
	}
	//Setup wall test, entity 4 
	{
		//Create new Entity 
		auto entity = ECS::CreateEntity();

		//Add components 
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		ECS::AttachComponent<PhysicsBody>(entity);
		//Sets up components 
		std::string fileName = "Box.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 200);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 98.f));
		//Grabs reference to various components 
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite 
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetHeight() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-240.f), float32(100.f));
		tempDef.fixedRotation = true;

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), true);

		//fixture definition 
		b2PolygonShape polygonShape;
		b2FixtureDef myFixtureDef;
		myFixtureDef.shape = &polygonShape;
		myFixtureDef.density = 999999.f;
		myFixtureDef.friction = 999999.f;
		//Adds a fixture the size of the body 
		polygonShape.SetAsBox(tempSpr.GetWidth(), tempSpr.GetHeight() / 2.f, b2Vec2(0, 0), 0);
		myFixtureDef.isSensor = true;
		b2Fixture* footSensorFixture = tempPhysBody.GetBody()->CreateFixture(&myFixtureDef);
		footSensorFixture->SetUserData((void*)3);

		//Sets up the identifier 
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Box1");
	}

	//Setup wall test, entity 5 
	{
		//Create new Entity 
		auto entity = ECS::CreateEntity();

		//Add components 
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		ECS::AttachComponent<PhysicsBody>(entity);
		//Sets up components 
		std::string fileName = "Box.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 200);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 98.f));
		//Grabs reference to various components 
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite 
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetHeight() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-350.f), float32(100.f));
		tempDef.fixedRotation = true;

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), true);

		//fixture definition 
		b2PolygonShape polygonShape;
		b2FixtureDef myFixtureDef;
		myFixtureDef.shape = &polygonShape;
		myFixtureDef.density = 999999.f;
		myFixtureDef.friction = 999999.f;
		//Adds a fixture the size of the body 
		polygonShape.SetAsBox(tempSpr.GetWidth(), tempSpr.GetHeight() / 2.f, b2Vec2(0, 0), 0);
		myFixtureDef.isSensor = true;
		b2Fixture* footSensorFixture = tempPhysBody.GetBody()->CreateFixture(&myFixtureDef);
		footSensorFixture->SetUserData((void*)3);

		//Sets up the identifier 
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Box1");
	}

	//Main Camera
	{//Creates camera entity
		auto entity = ECS::CreateEntity();
		EntityIdentifier::MainCamera(entity);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);

		vec4 temp = ECS::GetComponent<Camera>(entity).GetOrthoSize();
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<HorizontalScroll>(entity).SetOffset(15.f);
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetOffset(15.f);

		//Sets up identifier
		unsigned int bitHolder = EntityIdentifier::HoriScrollCameraBit() | EntityIdentifier::VertScrollCameraBit() | EntityIdentifier::CameraBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Main Scrolling Camera");
		ECS::SetIsMainCamera(entity, true);
	}
	//Makes the camera focus on the main player
	ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
}