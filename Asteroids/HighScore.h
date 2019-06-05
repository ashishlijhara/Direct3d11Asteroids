#pragma once
#include "GameState.h"
class HighScore :
	public GameState
{
public:
	HighScore();
	void OnActivate(System *system, StateArgumentMap &args);
	void OnUpdate(System *system);
	void OnRender(System *system);
	void OnDeactivate(System *system);

private:
	int delay_;
};

