#include "Game.h"

#include <random>

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
	BackEnd::InitBackEnd(1280.f, 720.f);

	//Grabs the initialized window
	m_window = BackEnd::GetWindow();

	//Creates a new scene.
	//Replace this with your own scene.

	m_scenes.push_back(new PhysicsTestScene("Physics Test Scene"));
	m_scenes.push_back(new Level1Scene("Level 1 Scene"));
	m_scenes.push_back(new Level1Scene("Chad poggers"));
	//m_scenes.push_back(new Level1Scene("Level 1 Scene"));

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

	//Updates the active scene
	m_activeScene->Update();

<<<<<<< HEAD
<<<<<<< HEAD
	for (b2ContactEdge* ce = m_register->get<PhysicsBody>(1).GetBody()->GetContactList(); ce; ce = ce->next)
	{
		b2Contact* c = ce->contact;

		if (c->IsTouching())
		{
		}
	}
=======
=======
	/*
	Some bullshit
>>>>>>> Noah_Branch

	MovementSystem Klock; //We will need a object for klock since the platform will move him too
	Klock.SetBothBodies(1);
	
<<<<<<< HEAD
>>>>>>> Noah_Branch
=======
	MovementSystem Level1Platform1;
	Level1Platform1.SetBothBodies(8);
	Level1Platform1.TeleportMovementRight(0.3f);
>>>>>>> Noah_Branch

	for (b2ContactEdge* ce = m_register->get<PhysicsBody>(8).GetBody()->GetContactList(); ce; ce = ce->next) //Platform Contact Check to see if we need to move klock
	{
		b2Contact* c = ce->contact;

		if (c->IsTouching())
		{
			Level1Platform1.SetIsTouching();
		}
		else
			Level1Platform1.SetIsTouching(false);
	}

	if (Level1Platform1.GetIsTouching())
		Klock.GetB2Body()->SetTransform(b2Vec2(0.3 + Klock.GetB2Body()->GetPosition().x, Klock.GetB2Body()->GetPosition().y), 0); //Not Sure why but the movement system stuff breaks here
	*/
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//     a.i     testing
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	All this stuff will be changed soon
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
<<<<<<< HEAD
		if (isColliding == true)
			tempPhysBod.ApplyForce(vec3(-1800000.f, playerBody->GetLinearVelocity().y * 5, 0.f));
		else
<<<<<<< HEAD
			tempPhysBod.ApplyForce(vec3(playerBody->GetLinearVelocity().x * 30, playerBody->GetLinearVelocity().y * 5, 0.f));
=======
			tempPhysBod.ApplyForce(vec3(-560000.f, 0.f, 0.f));
>>>>>>> Noah_Branch
	}
	if (Input::GetKey(Key::D))
	{
		if (isColliding == true)
			tempPhysBod.ApplyForce(vec3(1800000.f, playerBody->GetLinearVelocity().y * 5, 0.f));

		else
<<<<<<< HEAD
			tempPhysBod.ApplyForce(vec3(playerBody->GetLinearVelocity().x * 30, playerBody->GetLinearVelocity().y * 5, 0.f));
=======
			tempPhysBod.ApplyForce(vec3(560000.f, 0.f, 0.f));
>>>>>>> Noah_Branch
=======
		if (Klock.GetIsTouching())
		{
			Klock.MoveLeft(98000.f);
		}
		else
		{
			Klock.MoveLeft(16000.f);
		}

	}
	if (Input::GetKey(Key::D))
	{
		if (Klock.GetIsTouching())
			Klock.MoveRight(98000.f);
		else
			Klock.MoveRight(16000.f);
>>>>>>> Noah_Branch
	}

	//std::cout << "\nSpeed X: " << playerBody->GetLinearVelocity().x;
	//std::cout << "\nSpeed Y: " << playerBody->GetLinearVelocity().y;

	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->KeyboardHold();
}

void Game::KeyboardDown()
{
	MovementSystem Klock;
	Klock.SetBothBodies(1);
	
	Klock.SetIsTouching();//Updates the isTouching

	if (Input::GetKeyDown(Key::W))
	{
<<<<<<< HEAD
		if (isColliding == true)
			tempPhysBod.ApplyForce(vec3(playerBody->GetLinearVelocity().x * 10000000, 1300000000.f, 0.f));
		std::cout << "\nSpeed X: " << playerBody->GetLinearVelocity().x;
	}
	if (Input::GetKeyDown(Key::S))
	{
		if (!isColliding)
			tempPhysBod.ApplyForce(vec3(playerBody->GetLinearVelocity().x * 5, -999999999999.f, 0.f));
=======
		if (Klock.GetIsTouching())
			Klock.Jump(4300000.f);
	}
	if (Input::GetKeyDown(Key::S))
	{
		if (!Klock.GetIsTouching())
			Klock.DownMove(999999999999.f);
>>>>>>> Noah_Branch
	}

	m_activeScene->KeyboardDown();
}

void Game::KeyboardUp()
{
	m_activeScene->KeyboardUp();
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