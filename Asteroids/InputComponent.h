#pragma once
class Game;
class System;
class InputComponent
{
public:
	virtual ~InputComponent() {}
	virtual void Update(Game* game, System* system) = 0;
};

