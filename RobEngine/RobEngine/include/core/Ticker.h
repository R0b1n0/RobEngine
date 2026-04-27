#pragma once
#include "core/GameState.h"
#include <iostream>

class Ticker
{
public : 

	Ticker()
	{
		std::cout << "ticker \n";
		GameState::RegisterTickFunction([this](float dt){this->Update(dt);});
	}

	//Est ce que avoir Update en private peut quand même permettre de l'appeller
	//dps la gameloop?
	void virtual Update(float deltaTime) {};
};
