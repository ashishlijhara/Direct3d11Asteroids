#include "System.h"
#include "MainWindow.h"
#include "ResourceLoader.h"
#include "Graphics.h"
#include "AssetLoader.h"
#include "StateLibrary.h"
#include "Keyboard.h"
#include "GameState.h"
#include "Game.h"


System::System(HINSTANCE hInstance) :
	moduleInstance_(hInstance),
	mainWindow_(0),
	quit_(false),
	resourceLoader_(0),
	graphics_(0),
	assetLoader_(0),
	stateLibrary_(0),
	keyboard_(0),
	currentState_(0),
	nextState_(0),
	game_(0),
	scoreKeeper_(0)
{
}

System::~System()
{
}

void System::Initialise()
{
	mainWindow_ = new MainWindow(moduleInstance_);
	resourceLoader_ = new ResourceLoader();
	graphics_ = Graphics::CreateDevice(mainWindow_->GetHandle(), resourceLoader_);
	assetLoader_ = new AssetLoader();
	stateLibrary_ = new StateLibrary();
	keyboard_ = new Keyboard();
	game_ = new Game();
	scoreKeeper_ = Score::GetScoreKeeper();
}

void System::Test()
{
}

void System::Run()
{
	while (!quit_)
	{
		ProcessMessageQueue();
		SwapState();
		Update();
		Render();
	}

	if (currentState_ != 0)
	{
		currentState_->OnDeactivate(this);
	}
}

void System::Terminate()
{
	delete game_;
	game_ = 0;

	delete keyboard_;
	keyboard_ = 0;

	delete stateLibrary_;
	stateLibrary_ = 0;

	delete assetLoader_;
	assetLoader_ = 0;

	Graphics::DestroyDevice(graphics_);
	graphics_ = 0;

	delete resourceLoader_;
	resourceLoader_ = 0;

	delete mainWindow_;
	mainWindow_ = 0;

	Score::DestroyScoreKeeper();
	scoreKeeper_ = 0;
}

void System::SetScore(const int& level, const int& score) const
{
	scoreKeeper_->UpdateScore(level, score);
}

void System::SaveHighScoreTable() const {
	scoreKeeper_->SetHighScore();
}


ResourceLoader *System::GetResourceLoader() const
{
	return resourceLoader_;
}

Graphics *System::GetGraphics() const
{
	return graphics_;
}

AssetLoader *System::GetAssetLoader() const
{
	return assetLoader_;
}

Keyboard *System::GetKeyboard() const
{
	return keyboard_;
}

Game *System::GetGame() const
{
	return game_;
}

void System::SetNextState(const std::string &stateName)
{
	nextState_ = stateLibrary_->GetState(stateName);
	nextStateArgs_.clear();
}

void System::SetNextState(const std::string &stateName, const GameState::StateArgumentMap &args)
{
	nextState_ = stateLibrary_->GetState(stateName);
	nextStateArgs_ = args;
}

void System::ProcessMessageQueue()
{
	MSG message;
	ZeroMemory(&message, sizeof(message));
	if (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		switch (message.message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_QUIT:
			quit_ = true;
			break;

		default:
			; // Do nothing
		}

		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

void System::SwapState()
{
	if (nextState_ != 0)
	{
		if (currentState_ != 0)
		{
			currentState_->OnDeactivate(this);
		}
		currentState_ = nextState_;
		nextState_ = 0;
		currentState_->OnActivate(this, nextStateArgs_);
		nextStateArgs_.clear();
	}
}

void System::Update()
{
	old = newTime;
	
	assetLoader_->Update();
	keyboard_->Update();
	currentState_->OnUpdate(this);
	Sleep(1);
	newTime = std::chrono::high_resolution_clock::now();
	deltaTime_ = std::chrono::duration_cast<std::chrono::milliseconds>(newTime - old).count();
}

long long System::GetFrameDeltaTime() const 
{
	return deltaTime_;
}

void System::Render()
{
	graphics_->BeginFrame();
	currentState_->OnRender(this);
	graphics_->EndFrame();
}
