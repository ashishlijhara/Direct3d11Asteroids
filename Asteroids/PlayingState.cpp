#include "PlayingState.h"
#include "System.h"
#include "Game.h"

PlayingState::PlayingState()
{
}

PlayingState::~PlayingState()
{
}

void PlayingState::OnActivate(System *system, StateArgumentMap &args)
{
	Game *game = system->GetGame();

	level_ = args["Level"].asInt;
	game->InitialiseLevel(level_);
}

void PlayingState::OnUpdate(System *system)
{
	Game *game = system->GetGame();
	game->Update(system);
	if (game->IsLevelComplete())
	{
		StateArgumentMap args;
		args["Level"].asInt = level_ + 1;
		system->SetScore(level_+1, game->GetScore());
		system->SetNextState("LevelStart", args);
	}
	else if (game->IsGameOver())
	{
		system->SetScore(level_, game->GetScore());
		system->SetNextState("GameOver");
	}
}

void PlayingState::OnRender(System *system)
{
	Game *game = system->GetGame();
	game->RenderEverything(system->GetGraphics());
}

void PlayingState::OnDeactivate(System *system)
{
}
