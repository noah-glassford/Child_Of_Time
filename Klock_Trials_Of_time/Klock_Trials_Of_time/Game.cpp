#include "Game.h"

#include <random>


int Frames;

bool direction{ 0 }; //1 for right, 0 for left

bool slowSpamBlock = true;
bool ignoreThis = true;

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
	BackEnd::InitBackEnd(1920.f, 1080.f);

	SoundManager::init("./Assets/Sounds/");

	//Grabs the initialized window
	m_window = BackEnd::GetWindow();

	//initialise all the sound

	//Creates a new scene.
	//Replace this with your own scene.

	m_scenes.push_back(new MainMenu("Main Menu Scene")); //0
	m_scenes.push_back(new Level1Scene("Level 1 Scene")); //1
	m_scenes.push_back(new Level2Scene("Level 2 Scene")); //2
	m_scenes.push_back(new BossFightScene("Boss Fight Scene")); //3

	//Sets active scene reference to our scene
	m_activeScene = m_scenes[0];

	//m_activeScene->InitScene(float(BackEnd::GetWindowWidth()), float(BackEnd::GetWindowHeight()));
	m_activeScene->InitScene(float(BackEnd::GetWindowWidth()), float(BackEnd::GetWindowHeight()));

	ECS::GetComponent<PlayerData>(1).CurrentScene = 0;

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

#pragma region animationswitchstuff
	//Animation stuff
	//if (ECS::GetComponent<PhysicsBody>(1).GetBody()->GetLinearVelocity().x == 0)
		//ECS::GetComponent<AnimationController>(1).SetActiveAnim(0);
	if (ECS::GetComponent<AnimationController>(1).GetActiveAnim() == 2 && ECS::GetComponent<PhysicsBody>(1).GetBody()->GetLinearVelocity().x == 0)
		ECS::GetComponent<AnimationController>(1).SetActiveAnim(0);

	if (ECS::GetComponent<AnimationController>(1).GetActiveAnim() == 6 && ECS::GetComponent<PhysicsBody>(1).GetBody()->GetLinearVelocity().x == 0)
		ECS::GetComponent<AnimationController>(1).SetActiveAnim(4);

	if (ECS::GetComponent<AnimationController>(1).GetAnimation(ECS::GetComponent<AnimationController>(1).GetActiveAnim()).GetAnimationDone() && !ECS::GetComponent<PlayerData>(1).facingLeft)
	{
		ECS::GetComponent<AnimationController>(1).SetActiveAnim(0);
	}
	else if (ECS::GetComponent<AnimationController>(1).GetAnimation(ECS::GetComponent<AnimationController>(1).GetActiveAnim()).GetAnimationDone() && ECS::GetComponent<PlayerData>(1).facingLeft)
	{
		ECS::GetComponent<AnimationController>(1).SetActiveAnim(4);
	}

	if (ECS::GetComponent<PhysicsBody>(1).GetBody()->GetLinearVelocity().y > 0
		&& !ECS::GetComponent<PlayerData>(1).facingLeft && !ECS::GetComponent<PlayerData>(1).Grounded && ECS::GetComponent<AnimationController>(1).GetActiveAnim() != 3)
	{
		ECS::GetComponent<AnimationController>(1).SetActiveAnim(1);
	}
	else if (ECS::GetComponent<PhysicsBody>(1).GetBody()->GetLinearVelocity().y > 0
		&& ECS::GetComponent<PlayerData>(1).facingLeft && !ECS::GetComponent<PlayerData>(1).Grounded && ECS::GetComponent<AnimationController>(1).GetActiveAnim() != 7)
	{
		ECS::GetComponent<AnimationController>(1).SetActiveAnim(5);
	}

	if (!ECS::GetComponent<PlayerData>(1).Grounded && !ECS::GetComponent<PlayerData>(1).facingLeft&& ECS::GetComponent<AnimationController>(1).GetAnimation(ECS::GetComponent<AnimationController>(1).GetActiveAnim()).GetAnimationDone()
		)
	{
		ECS::GetComponent<AnimationController>(1).SetActiveAnim(1);
	}
	else if (!ECS::GetComponent<PlayerData>(1).Grounded&& ECS::GetComponent<PlayerData>(1).facingLeft&& ECS::GetComponent<AnimationController>(1).GetAnimation(ECS::GetComponent<AnimationController>(1).GetActiveAnim()).GetAnimationDone()
		)
	{
		ECS::GetComponent<AnimationController>(1).SetActiveAnim(5);
	}

#pragma endregion

	//This switches scene at the end of level 1
	if (ECS::GetComponent<PlayerData>(1).CurrentScene == 1)
		if (ECS::GetComponent<PhysicsBody>(35).GetBody()->GetContactList() != 0 && ECS::GetComponent<PlayerData>(1).canUseTimeSlow == 1)
			Switchscene(2);
	if (ECS::GetComponent<PlayerData>(1).CurrentScene == 2)
		if (ECS::GetComponent<PhysicsBody>(49).GetBody()->GetContactList() != 0 && ECS::GetComponent<PlayerData>(1).canUseTimeSlow == 1)
			Switchscene(3);

	

	//std::cout << UsedUpTime << " " << isSlowed << std::endl;
	unsigned int mainp = EntityIdentifier::MainPlayer();

	if (ECS::GetComponent<PlayerData>(mainp).UsedUpTime > 0 && !ECS::GetComponent<PlayerData>(1).isSlowed)
		ECS::GetComponent<PlayerData>(mainp).UsedUpTime = ECS::GetComponent<PlayerData>(1).UsedUpTime - deltaTime * 2;

	if (ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).isSlowed)
	{
		if (ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).UsedUpTime <= 10.f)
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).UsedUpTime = ECS::GetComponent<PlayerData>(1).UsedUpTime + deltaTime;

		if (ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).UsedUpTime < 10.f)
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).isSlowed = true;

		else if (ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).UsedUpTime > 10.f)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).isSlowed = false;
			EffectManager::RemoveEffect(0);
		}
	}

	if (ECS::GetComponent<PlayerData>(mainp).TimeSinceHit > 0)
	{
		ECS::GetComponent <PlayerData>(mainp).TimeSinceHit -= deltaTime;
	}
	else
	{
		ECS::GetComponent<PlayerData>(mainp).Hit = 1;
	}

	if (ECS::GetComponent<PlayerData>(mainp).TimeSinceAtt > 0)
	{
		ECS::GetComponent<PlayerData>(mainp).TimeSinceAtt -= deltaTime;
		ECS::GetComponent<PlayerData>(mainp).CanAttack = 0;
	}
	else
	{
		ECS::GetComponent<PlayerData>(mainp).CanAttack = 1;
		//		ECS::GetComponent<AnimationController>(1).GetAnimation(3).SetRepeating(false);
	}
	//The unironic worst way to do this (scene switching code)
	/*
	Commented out until we know what coords level 1 ends at
	CLARK IF YOU TOUCH THIS I WILL DRIVE TO OSHAWA AND COUGH ON YOU
	if (ECS::GetComponent<PhysicsBody>(1).GetPosition().x > 400) //This is where we put scene switch condition
	{
		if (ECS::GetComponent<PlayerData>(1).CurrentScene == 1)
		{
			//change scene
			m_activeScene = m_scenes[2];

			m_activeScene->InitScene(float(BackEnd::GetWindowWidth()), float(BackEnd::GetWindowHeight()));

			m_register = m_activeScene->GetScene();
		}
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

void Game::Switchscene(int scene)
{
	SoundManager::stop();

	m_activeScene->Unload();

	m_activeScene = m_scenes[scene];

	m_activeScene->InitScene(float(BackEnd::GetWindowWidth()), float(BackEnd::GetWindowHeight()));

	m_register = m_activeScene->GetScene();
}

void Game::MainMenuControlls(SDL_MouseButtonEvent event)
{
	float windowWidth = BackEnd::GetWindowWidth();
	float windowHeight = BackEnd::GetWindowHeight();

	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		vec3(click) = vec3((event.x / windowWidth) * 100, (event.y / windowHeight) * 100, 0.f);
		std::cout << click.x << "\n" << click.y << "\n";

		if (click.x < 57 && click.x > 52 && click.y > 45 && click.y < 54)
		{
			Switchscene(1);
		}
	}
}

void Game::CheckEvents()
{
	if (ECS::GetComponent<PlayerData>(1).CurrentScene == 0)
		MainMenuControlls(BackEnd::GetClickEvent());

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
	MovementSystem Klock;;
	Klock.SetBothBodies(1);
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	if (con->IsButtonStroked(Buttons::RB)) //Combat
	{
		ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).isAttacking = true;
		std::cout << "bruh";
	}
	else
		ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).isAttacking = false;

	m_activeScene->GamepadStroke(con);
}

void Game::GamepadUp(XInputController* con)
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->GamepadUp(con);
	if (con->IsButtonReleased(Buttons::X))
		slowSpamBlock = true;
}

void Game::GamepadDown(XInputController* con)
{
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	MovementSystem Klock;
	Klock.SetBothBodies(1);

	if (con->IsButtonPressed(Buttons::DPAD_RIGHT))
	{
		if (!ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).OnWallRight)
			Klock.MoveRight(30.f);

		//ECS::GetComponent<AnimationController>(EntityIdentifier::MainPlayer()).SetActiveAnim(0);
	}
	if (con->IsButtonPressed(Buttons::DPAD_LEFT))
	{
		if (!ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).OnWallLeft)
			Klock.MoveLeft(30.f);

		//ECS::GetComponent<AnimationController>(EntityIdentifier::MainPlayer()).SetActiveAnim(1);
	}

	if (con->IsButtonPressed(Buttons::A))
	{
		if (ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).Grounded) {
			Klock.Jump(520000.f);
			Sound2D _jump("jump.wav", "group1");
			_jump.play();
		}
	}
	m_activeScene->GamepadDown(con);

	if (con->IsButtonPressed(Buttons::X) && slowSpamBlock)
	{
		slowSpamBlock = false;
		Sound2D _TimeStop("timestop.wav", "group1");
		Sound2D _TimeRestart("timeresume.wav", "group1");

		if (ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).isSlowed == false)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).isSlowed = true;
			_TimeStop.play();
		}
		else {
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).isSlowed = false;
			_TimeRestart.play();
		}
	}
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
	//Make sure that the player is always entity 1

	MovementSystem Klock; //Handles all the movement functions for Klock
	Klock.SetBothBodies(EntityIdentifier::MainPlayer());

	Klock.SetIsTouching(); //Klock specific contact updating

	if (Input::GetKey(Key::S))
	{
		//Crouching will be done here
	}
	if (Input::GetKey(Key::A))
	{
		if (!ECS::GetComponent<PlayerData>(1).OnWallLeft&& ECS::GetComponent<PlayerData>(1).Hit)
			Klock.MoveLeft(40.f);
		ECS::GetComponent<PlayerData>(1).facingLeft = 1;
		//if (!ECS::GetComponent<AnimationController>(1).GetActiveAnim() == 3)
		if (ECS::GetComponent<PlayerData>(1).Grounded&& ECS::GetComponent<AnimationController>(1).GetActiveAnim() != 7)
			ECS::GetComponent<AnimationController>(1).SetActiveAnim(6);
	}
	if (Input::GetKey(Key::D))
	{
		if (!ECS::GetComponent<PlayerData>(1).OnWallRight&& ECS::GetComponent<PlayerData>(1).Hit)
			Klock.MoveRight(40.f);

		ECS::GetComponent<PlayerData>(1).facingLeft = 0;
		//if (!ECS::GetComponent<AnimationController>(1).GetActiveAnim() == 3)
		if (ECS::GetComponent<PlayerData>(1).Grounded&& ECS::GetComponent<AnimationController>(1).GetActiveAnim() != 3)
			ECS::GetComponent<AnimationController>(1).SetActiveAnim(2);
	}
	else
	{
	}
	if (Input::GetKey(Key::O))
		ECS::GetComponent<Camera>(EntityIdentifier::MainCamera()).Zoom(-5);
	if (Input::GetKey(Key::I))
		ECS::GetComponent<Camera>(EntityIdentifier::MainCamera()).Zoom(5);
	//Active scene now captures this input and can use it
	//Look at base Scene class for more info.
	m_activeScene->KeyboardHold();
}

void Game::KeyboardDown()
{
	MovementSystem Klock;
	Klock.SetBothBodies(EntityIdentifier::MainPlayer());

	if (Input::GetKeyDown(Key::W))
	{
		if (ECS::GetComponent<PlayerData>(1).Grounded)
		{
			Klock.Jump(4200000.f);
			Sound2D _jump("jump.wav", "group1");
			_jump.play();
			//ECS::GetComponent<AnimationController>(1).GetAnimation(1).Reset();
			//ECS::GetComponent<AnimationController>(1).GetAnimation(5).Reset();
			if (!ECS::GetComponent<PlayerData>(1).facingLeft)
				ECS::GetComponent<AnimationController>(1).SetActiveAnim(1);
			else
				ECS::GetComponent<AnimationController>(1).SetActiveAnim(5);
		}
	}
	if (Input::GetKeyDown(Key::K) && ECS::GetComponent<PlayerData>(1).CanAttack)
	{
		ECS::GetComponent<AnimationController>(1).GetAnimation(3).Reset();
		ECS::GetComponent<AnimationController>(1).GetAnimation(7).Reset();
		if (!ECS::GetComponent<PlayerData>(1).facingLeft)
		{
			ECS::GetComponent<AnimationController>(1).SetActiveAnim(3);
		}
		else
		{
			ECS::GetComponent<AnimationController>(1).SetActiveAnim(7);
		}

		ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).isAttacking = true;
	}
	else
	{
		ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).isAttacking = false;
	}

	if (Input::GetKeyDown(Key::L) && slowSpamBlock && ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).canUseTimeSlow)
	{
		slowSpamBlock = false;
		Sound2D _TimeStop("timestop.wav", "group1");
		Sound2D _TimeRestart("timeresume.wav", "group1");
		if (ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).isSlowed == false)
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).isSlowed = true;
			_TimeStop.play();
		}
		else
		{
			ECS::GetComponent<PlayerData>(EntityIdentifier::MainPlayer()).isSlowed = false;
			_TimeRestart.play();
		}
		if (ECS::GetComponent<PlayerData>(1).isSlowed)
			EffectManager::CreateEffect(Vignette, BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());
		else
			EffectManager::RemoveEffect(0);
	}

	if (Input::GetKeyDown(Key::Escape) && ECS::GetComponent<PlayerData>(1).CurrentScene == 3 && ECS::GetComponent<PlayerData>(2).Health == 0)
		Switchscene(0);

	if (Input::GetKeyDown(Key::Enter) && ECS::GetComponent<PlayerData>(1).CurrentScene == 0)
	{
		//	ECS::GetComponent<Transform>(3).SetPositionX(0.f);
		
		ECS::GetComponent<Transform>(3).SetPosition(vec3(0, 0, 5));
		//if (ECS::GetComponent<PlayerData>(1).CurrentScene == 0)
			//Switchscene(1);
	}

	if (Input::GetKeyDown(Key::Alt) && ECS::GetComponent<PlayerData>(1).CurrentScene == 1)
	{
		//	ECS::GetComponent<Transform>(3).SetPositionX(0.f);
		Switchscene(1);
		//if (ECS::GetComponent<PlayerData>(1).CurrentScene == 0)
			//Switchscene(1);
	}

	
	if (Input::GetKeyDown(Key::Alt) && ECS::GetComponent<PlayerData>(1).CurrentScene == 2)
	{
		//	ECS::GetComponent<Transform>(3).SetPositionX(0.f);
		Switchscene(2);
		//if (ECS::GetComponent<PlayerData>(1).CurrentScene == 0)
			//Switchscene(1);
	}

	if (Input::GetKeyDown(Key::Alt) && ECS::GetComponent<PlayerData>(1).CurrentScene == 3)
	{
		//	ECS::GetComponent<Transform>(3).SetPositionX(0.f);
		Switchscene(3);
		//if (ECS::GetComponent<PlayerData>(1).CurrentScene == 0)
			//Switchscene(1);
	}

	


	m_activeScene->KeyboardDown();
}

void Game::KeyboardUp()
{
	if (Input::GetKeyUp(Key::L)) {
		slowSpamBlock = true;
	}
	if (Input::GetKeyUp(Key::Enter) && ECS::GetComponent<PlayerData>(1).CurrentScene == 0)
		Switchscene(1);


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