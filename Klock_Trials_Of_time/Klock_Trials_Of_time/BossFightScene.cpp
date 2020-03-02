#include "BossFightScene.h"

BossFightScene::BossFightScene(std::string name)
{
}

void BossFightScene::InitScene(float windowWidth, float windowHeight)
{
}

void BossFightScene::Update()
{
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

	//Makes the camera focus on the main player
	ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));


}
