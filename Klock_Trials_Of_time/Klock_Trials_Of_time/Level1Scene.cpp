#include "Level1Scene.h"


Level1Scene::Level1Scene(std::string name)
	: Scene(name)
{
	m_gravity = b2Vec2(float32(0.f), float32(-150.f));
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

	//Setup box #1, entity 1
	{
		//Create new Entity 
		auto entity = ECS::CreateEntity();
		//EntityIdentifier::MainPlayer(entity); 

		//Add components 
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		ECS::AttachComponent<PhysicsBody>(entity);
		//Sets up components 
		std::string fileName = "Klock_Png.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 50);
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
		tempDef.fixedRotation = true;
		tempDef.position.Set(float32(26.f), float32(50.f));
		tempDef.gravityScale = 1.5;
		
		
		

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), true);


		//fixture definition
		b2PolygonShape polygonShape;
		b2FixtureDef myFixtureDef;
		myFixtureDef.shape = &polygonShape;
		myFixtureDef.density = 50;

		//add foot sensor fixture
		polygonShape.SetAsBox(tempSpr.GetWidth(), tempSpr.GetHeight(), b2Vec2(0, 0), 0);
		//myFixtureDef.isSensor = true;
		b2Fixture* footSensorFixture = tempPhysBody.GetBody()->CreateFixture(&myFixtureDef);
		footSensorFixture->SetUserData((void*)3);


		//Sets up the identifier 
		ECS::SetIsMainPlayer(entity, true);
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Box1");
	}
		
	
	//Background and Ground Object entity 0
	{
		//Create new entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "ConceptArt_Background.png";
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



			//fixture definition
			b2PolygonShape polygonShape;
			b2FixtureDef myFixtureDef;
			myFixtureDef.shape = &polygonShape;
			myFixtureDef.density = 1;
			myFixtureDef.friction = 1.f;
			//Adds a fixture the size of the body
			polygonShape.SetAsBox(tempSpr.GetWidth(), tempSpr.GetHeight() / 2.f, b2Vec2(0, 0), 0);
			myFixtureDef.isSensor = true;
			b2Fixture* footSensorFixture = tempPhysBody.GetBody()->CreateFixture(&myFixtureDef);
			footSensorFixture->SetUserData((void*)3);

			//Sets up the Identifier
			unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
			ECS::SetUpIdentifier(entity, bitHolder, "BackGround");
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

