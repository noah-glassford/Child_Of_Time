#include "AssignmentScene.h"
AssignemntScene::AssignemntScene(std::string name)
	: Scene(name)
{
	m_gravity = b2Vec2(float32(0.f), float32(-9.f));
	m_physicsWorld->SetGravity(m_gravity);
}

void AssignemntScene::InitScene(float windowWidth, float windowHeight)
{
	//Allocates Register
	m_sceneReg = new entt::registry;
	
	//Attach the Register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;
	
	
	//Setup new Entity (Background Sprite, Entity 1)
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "Background_Alien.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 940, 500);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 1.f));

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);
		
		//Physics body covers half the sprite
		//Id type is environment
		float shrinkX = 0.f;
		float shrinkY = (tempSpr.GetHeight() / 2.f);

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(0.f), float32(50.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY),
			vec2(0.f, (-tempSpr.GetHeight() / 16.f) * 6.f), false);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "BackGround");
	}
	
	//Setup box #1, entity 4
	{
		//Create new Entity
		auto entity = ECS::CreateEntity();
		//EntityIdentifier::MainPlayer(entity);

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		
		ECS::AttachComponent<PhysicsBody>(entity);
		//Sets up components
		std::string fileName = "Box.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 25);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(26.f, 0.f, 97.f));
		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);
		
		//Physics body covers the entire sprite
		float shrinkX = tempSpr.GetWidth() / 2.f ;
		float shrinkY = tempSpr.GetHeight() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(26.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), true);
		

	
		//Sets up the identifier
		
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Box1");
		
	}
	//Setup box #2, entity 5
	{
		//Create new Entity
		auto entity = ECS::CreateEntity();
		//EntityIdentifier::MainPlayer(entity);

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		ECS::AttachComponent<PhysicsBody>(entity);
		//Sets up components
		std::string fileName = "Box.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 25);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 97.f));
		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		float shrinkX = tempSpr.GetWidth() / 2.f ;
		float shrinkY = tempSpr.GetHeight() / 2.f ;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), true);


		
		//Sets up the identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Box2");
	}
	//Setup box #3, entity 6
	{
		//Create new Entity
		auto entity = ECS::CreateEntity();
		//EntityIdentifier::MainPlayer(entity);

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		ECS::AttachComponent<PhysicsBody>(entity);
		//Sets up components
		std::string fileName = "Box.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 25);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-26.f, 0.f, 97.f));
		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		float shrinkX = tempSpr.GetWidth() /2.f ;
		float shrinkY = tempSpr.GetHeight() /2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-26.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), true);


		//Sets up the identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Box3");
	}
	//Setup box #4, entity 7
	{
		//Create new Entity
		auto entity = ECS::CreateEntity();
		//EntityIdentifier::MainPlayer(entity);

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		ECS::AttachComponent<PhysicsBody>(entity);
		//Sets up components
		std::string fileName = "Box.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 25);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(15.f, 26.f, 97.f));
		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		float shrinkX = tempSpr.GetWidth() /2.f;
		float shrinkY = tempSpr.GetHeight()  /2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(15.f), float32(26.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), true);


		
		//Sets up the identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Box4");
	}
	//Setup box #5, entity 8
	{
		//Create new Entity
		auto entity = ECS::CreateEntity();
		//EntityIdentifier::MainPlayer(entity);

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		ECS::AttachComponent<PhysicsBody>(entity);
		//Sets up components
		std::string fileName = "Box.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 25);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-15.f, 26.f, 97.f));
		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		float shrinkX = tempSpr.GetWidth() /2.f;
		float shrinkY = tempSpr.GetHeight() /2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-15.f), float32(26.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), true);


		
		//Sets up the identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Box5");
	}
	//Setup box #6, entity 9
	{
		//Create new Entity
		auto entity = ECS::CreateEntity();
		//EntityIdentifier::MainPlayer(entity);

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		ECS::AttachComponent<PhysicsBody>(entity);
		//Sets up components
		std::string fileName = "Box.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 25);

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		float shrinkX = tempSpr.GetWidth() /2.f;
		float shrinkY = tempSpr.GetHeight() /2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0.f), float32(52.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), true);


		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 97.f));
		//Sets up the identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Box6");
	}

	//Setup box #7, entity 10
	{
		//Create new Entity
		auto entity = ECS::CreateEntity();
		//EntityIdentifier::MainPlayer(entity);

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		ECS::AttachComponent<PhysicsBody>(entity);
		//Sets up components
		std::string fileName = "Box.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 25);

		//Grabs reference to various components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		float shrinkX = tempSpr.GetWidth() /2.f;
		float shrinkY = tempSpr.GetHeight() /2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-20.f), float32(100.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), false);


		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-20.f, 1000.f, 97.f));
		//Sets up the identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Box7");
		ECS::SetIsMainPlayer(entity, true);
	}
	
	//Setup main camera entity, entity 
	{
		//Creates camera entity
		auto entity = ECS::CreateEntity();
		EntityIdentifier::MainCamera(entity);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);
		vec4 temp = ECS::GetComponent<Camera>(entity).GetOrthoSize();
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);
		//Ataches the camera to the scroll
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<HorizontalScroll>(entity).SetOffset(15.f);
		
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetOffset(15.f);
		//Sets up identifier
		unsigned int bitHolder = EntityIdentifier::HoriScrollCameraBit() | EntityIdentifier::VertScrollCameraBit() | EntityIdentifier::CameraBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Horizontal And Vertical Scrolling Cam");
		ECS::SetIsMainCamera(entity, true);
	}
	//Makes the camera focus on the main player
	//We do this at the very bottom so we get the most accurate pointer to our Transform
	ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
}