#include "Game.h"

#include <random>

//Yo can I just put a bool here for it to be global lmao
//Fuck it float here too this is probably bad but I don't care
//Both of these are used for the time slowing stuff because they are used in multiple functions in game.cpp
//so I just said fuck it and made them global
bool isSlowed;
float UsedUpTime{ 0 };
bool direction{ 0 }; //1 for right, 0 for left

Game::~Game()
{
	//If window isn't equal to nullptr
	if (m_window != nullptr)
	{
		//Delete window
		delete m_window;
		//set window to nullptr
		m_window = nullptr;
	}

	//Goes through the scenes and deletes them
	for (unsigned i = 0; i < m_scenes.size(); i++)
	{
		if (m_scenes[i] != nullptr)
		{
			delete m_scenes[i];
			m_scenes[i] = nullptr;
		}
	}
}

void Game::InitGame()
{
	//Initializes the backend with window width and height values
	BackEnd::InitBackEnd(500.f, 500.f);

	//Grabs the initialized window
	m_window = BackEnd::GetWindow();

	//Creates a new scene.
	//Replace this with your own scene.

	m_scenes.push_back(new PhysicsTestScene("Physics Test Scene")); //0
	m_scenes.push_back(new Level1Scene("Level 1 Scene")); //1
	m_scenes.push_back(new Level2Scene("Level 2 Scene")); //2

		//Sets active scene reference to our scene
	m_activeScene = m_scenes[1];

	//m_activeScene->InitScene(float(BackEnd::GetWindowWidth()), float(BackEnd::GetWindowHeight()));
	m_activeScene->InitScene(float(BackEnd::GetWindowWidth()), float(BackEnd::GetWindowHeight()));

	//Sets m_register to point to the register in the active scene
	m_register = m_activeScene->GetScene();

	BackEnd::SetWindowName(m_activeScene->GetName());

	PhysicsSystem::Init();
}

bool Game::Run()
{
	//While window is still open
	while (m_window->isOpen())
	{
		//Clear window with activescene clearColor
		m_window->Clear(m_activeScene->GetClearColor());
		//Updates the game
		Update();
		//Draws the game
		BackEnd::Draw(m_register);

		//Draws ImGUI
		if (m_guiActive)
			GUI();

		//Flips the windows
		m_window->Flip();

		//Polls events and then checks them
		BackEnd::PollEvents(m_register, &m_close, &m_motion, &m_click, &m_wheel);
		CheckEvents();

		//does the window have keyboard focus?
		if (Input::m_windowFocus)
		{
			//Accept all input
			AcceptInput();
		}
	}

	return true;
}

void Game::Update()
{
	//Update timer
	Timer::Update();
	//Update the backend
	BackEnd::Update(m_register);

	//Update Physics System
	PhysicsSystem::Update(m_register, m_activeScene->GetPhysicsWorld());

	//std::cout << UsedUpTime << " " << isSlowed << std::endl;
	
	if (UsedUpTime > 0)
		UsedUpTime = UsedUpTime - deltaTime / 3;

	
	
	//Used to set direction

	if (ECS::GetComponent<PhysicsBody>(8).GetBody()->GetPosition().x > 800)
		direction = 0;
	else if (ECS::GetComponent<PhysicsBody>(8).GetBody()->GetPosition().x < 400)
		direction = 1;

	//std::cout << ECS::GetComponent<PhysicsBody>(8).GetBody()->GetPosition().x << std::endl;
	//Anything that can be affected by the time controls is done in this if statement
	if (!isSlowed)
	{
		if (direction)
			ECS::GetComponent<PhysicsBody>(8).GetBody()->SetLinearVelocity(b2Vec2(50.f, 0.f));
		if (!direction)
			ECS::GetComponent<PhysicsBody>(8).GetBody()->SetLinearVelocity(b2Vec2(-50.f, 0.f));
	}
	else if (isSlowed)
	{
		if (direction)
			ECS::GetComponent<PhysicsBody>(8).GetBody()->SetLinearVelocity(b2Vec2(10.f, 0.f));
		if (!direction)
			ECS::GetComponent<PhysicsBody>(8).GetBody()->SetLinearVelocity(b2Vec2(-10.f, 0.f));
	}
		
	

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//     a.i     testing
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	All this stuff will be changed soon bu tkeeping the code here because it do kinda be working tho
	auto& AIBodDefault = ECS::GetComponent<PhysicsBody>(2);
	auto& AIBodSprinter = ECS::GetComponent<PhysicsBody>(3);

	float distance1 = AIBodDefault.GetPosition().x - playerBod.GetPosition().x;
	float distance2 = AIBodSprinter.GetPosition().x - playerBod.GetPosition().x;

	//default enemy
	if (distance1 < 165 && distance1 > -165) {
		if (distance1 > 40)
			AIBodDefault.ApplyForce(vec3(-89999.f, 0.f, 0.f));
		if (distance1 < -40)
			AIBodDefault.ApplyForce(vec3(89999.f, 0.f, 0.f));
	}
	//sprinter enemy
	if (distance2 < 120 && distance2 > -120) {
		if (distance2 > 0)
			AIBodSprinter.ApplyForce(vec3(-150000.f, 0.f, 0.f));
		if (distance2 < 0)
			AIBodSprinter.ApplyForce(vec3(150000.f, 0.f, 0.f));
	}
	*/

	
	m_activeScene->Update();
}

void Game::GUI()
{
	UI::Start(BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());

	ImGui::Text("Place your different tabs below.");

	if (ImGui::BeginTabBar(""))
	{
		BackEnd::GUI(m_register, m_activeScene);

		ImGui::EndTabBar();
	}

	UI::End();
}

void Game::CheckEvents()
{
	if (m_close)
		m_window->Close();

	if (m_motion)
		MouseMotion(BackEnd::GetMotionEvent());

	if (m_click)
		MouseClick(BackEnd::GetClickEvent());

	if (m_wheel)
		MouseWheel(BackEnd::GetWheelEvent());
}

void Game::AcceptInput()
{
	XInputManager::Update();

	//Just calls all the other input functions
	GamepadInput();

	KeyboardHold();
	KeyboardDown();
	KeyboardUp();

	//Resets the key flags
	//Must be done once per frame for input to work
	Input::ResetKeys();
}

void Game::GamepadInput()
{
	XInputController* tempCon;
	//Gamepad button stroked (pressed)
	for (int i = 0; i < 3; i++)
	{
		if (XInputManager::ControllerConnected(i))
		{
			tempCon = XInputManager::GetController(i);
			tempCon->SetStickDeadZone(0.1f);

			//If the controller is connected, we run the different input types
			GamepadStroke(tempCon);
			GamepadUp(tempCon);
			GamepadDown(tempCon);
			GamepadStick(tempCon);
			GamepadTrigger(tempCon);
		}
	}
}

void Game::GamepadStroke(XInputController* con)
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->GamepadStroke(con);
}

void Game::GamepadUp(XInputController* con)
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->GamepadUp(con);
}

void Game::GamepadDown(XInputController* con)
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->GamepadDown(con);
}

void Game::GamepadStick(XInputController* con)
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->GamepadStick(con);
}

void Game::GamepadTrigger(XInputController* con)
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->GamepadTrigger(con);
}

void Game::KeyboardHold()
{
	MovementSystem Klock; //Handles all the movement functions for Klock
	Klock.SetBothBodies(1);
	
	Klock.SetIsTouching(); //Klock specific contact updating

	if (Input::GetKey(Key::S))
	{
		//Crouching will be done here
	}
	if (Input::GetKey(Key::A))
	{
		if (!Klock.GetPhysicsBody().OnWallLeft)
		Klock.MoveLeft(30.f);
	}
	if (Input::GetKey(Key::D))
	{
		if (!Klock.GetPhysicsBody().OnWallRight)
			Klock.MoveRight(30.f);	
	}

	if (Input::GetKey(Key::E))
	{
		if (UsedUpTime <= 2.f)
			UsedUpTime = UsedUpTime + deltaTime;
			
		if (UsedUpTime < 2.f)
			isSlowed = true;
		else if (UsedUpTime > 2.f)
			isSlowed = false;
	}
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->KeyboardHold();
}

void Game::KeyboardDown()
{
	MovementSystem Klock;
	Klock.SetBothBodies(1); //Overcomplicated shit
	
	Klock.SetIsTouching();//Updates the isTouching

	if (Input::GetKeyDown(Key::W))
	{
		if (Klock.GetIsTouching())
			Klock.Jump(3000000.f);
	}
	if (Input::GetKeyDown(Key::S))
	{
		if (!Klock.GetIsTouching())
			Klock.DownMove(999999999999.f);
	}

	m_activeScene->KeyboardDown();
}

void Game::KeyboardUp()
{
	if (Input::GetKeyUp(Key::E))
		isSlowed = 0;
	m_activeScene->KeyboardUp();

	if (Input::GetKeyUp(Key::I))
		ECS::GetComponent<Camera>(11).Zoom(-50);
}

void Game::MouseMotion(SDL_MouseMotionEvent evnt)
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->MouseMotion(evnt);

	if (m_guiActive)
	{
		ImGui::GetIO().MousePos = ImVec2(float(evnt.x), float(evnt.y));

		if (!ImGui::GetIO().WantCaptureMouse)
		{
		}
	}

	//Resets the enabled flag
	m_motion = false;
}

void Game::MouseClick(SDL_MouseButtonEvent evnt)
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->MouseClick(evnt);

	if (m_guiActive)
	{
		ImGui::GetIO().MousePos = ImVec2(float(evnt.x), float(evnt.y));
		ImGui::GetIO().MouseDown[0] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT));
		ImGui::GetIO().MouseDown[1] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT));
		ImGui::GetIO().MouseDown[2] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE));
	}

	//Resets the enabled flag
	m_click = false;
}

void Game::MouseWheel(SDL_MouseWheelEvent evnt)
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->MouseWheel(evnt);

	if (m_guiActive)
	{
		ImGui::GetIO().MouseWheel = float(evnt.y);
	}
	//Resets the enabled flag
	m_wheel = false;
}